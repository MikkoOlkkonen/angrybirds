#pragma once

#include "gamestate.hpp"
#include "highscores.hpp"
#include "collisiondetection.hpp"
#include <cmath>
#include <memory>

    class SandboxState : public GameState {
    public:
        /**
         * @brief Construct a new Sandbox State object
         * set the gravity and the physics world with that gravity, highscores objects, level number, whether the level is saved and zoom
         */
        SandboxState() : gravity_(0.0f, 9.8f), world_(gravity_), highscores_(HighScores()), level_number_(4), saved_(false), currentZoom_(0) {
            if (!soundBuffer_.loadFromFile("../src/soundfiles/binsound.wav")) {
                // throw error
                throw std::runtime_error("Error loading sound file: binsound.wav !");
            }
            sound_.setBuffer(soundBuffer_);
            sound_.setVolume(50);
            LevelData data(level_number_);
            initButtons();
            initMusic();
            for (auto& bird : data.getBirds()) 
            {   
                bird->initializePhysicsWorld(world_); // lisää birdin b2 maailmaan
                birds_.push_back(bird);
            }
            birdcount_ = birds_.size();
            for (auto& pig : data.getPigs())
            {
                pig->initializePhysicsWorld(world_); // lisää pigin b2 maailmaan
                pigs_.push_back(pig);
                }
            for (auto& obstacle : data.getObstacles())
            {
                obstacle->initializePhysicsWorld(world_); // lisää obstaclen b2 maailmaan
                obstacles_.push_back(obstacle);
            }

            ground_ = data.getGround();
            ground_->initializePhysicsWorld(world_); // Lisää groundin b2 maailmaan
            ground_->setData(); // lisää groundille tiedon siitä, että se on "ground"
            ground_->moveBodyDown(0.05f); // move ground body down 5 pixels so obejects do not levitate
            star_ = data.getStar();
            if (star_)
            {
                star_->initializePhysicsWorld(world_);
                star_->setData();
                star_->setBodyStatic();
                star_->getBody()->GetFixtureList()->SetSensor(true);
            }
        }

        ~SandboxState() {
            stopMusic();
        }

        /**
         * @brief Initialize buttons
         * 
         */
        void initButtons() {
            int width = 130;
            int height = 55;
            int space = 20;
            int pos_x = getWindowWidth() - width - space;        // places button to right of window horizontally
            int pos_y = getWindowHeight() - height - space;       // places button to right of window vertically
            
            buttons_.push_back(std::make_shared<Button>(pos_x, pos_y, width, height, "Save", std::pair<std::string,int>("save", 0), false, false));
            buttons_.push_back(std::make_shared<Button>(pos_x, pos_y - height - space, width, height, "Menu", std::pair<std::string,int>("menu", 0), false, false));
            buttons_.push_back(std::make_shared<Button>(pos_x, pos_y - 2 * height - 2 * space, width, height, "Play", std::pair<std::string,int>("open", level_number_), false, false));

            buttons_.push_back(std::make_shared<Button>(0, 0, width, height, "../src/imagefiles/redbird.png", std::pair<std::string,int>("red", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(width + space, 0, width, height, "../src/imagefiles/yellowbird.png", std::pair<std::string,int>("yellow", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(2 * width + 2 * space, 0, width, height, "../src/imagefiles/pig.png", std::pair<std::string,int>("pig", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(3 * width + 3 * space, 0, width, height, "../src/imagefiles/kingpig.png", std::pair<std::string,int>("king", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(4 * width + 4 * space, 0, width, height, "../src/imagefiles/wood.png", std::pair<std::string,int>("wood", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(5 * width + 5 * space, 0, width, height, "../src/imagefiles/stone.png", std::pair<std::string,int>("stone", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(6 * width + 6 * space, 0, width, height, "../src/imagefiles/glass.png", std::pair<std::string,int>("glass", 0), true, false));
            buttons_.push_back(std::make_shared<Button>(7 * width + 7 * space, 0, width, height, "../src/imagefiles/star.png", std::pair<std::string,int>("star", 0), true, false));
            bin_button_ = std::make_shared<Button>(0, pos_y-height+space, 2*width, 2*height, "Bin", std::pair<std::string,int>("bin", level_number_), false, false);
            bin_button_->changeToRed();
        }

        void initMusic() {
            if (!music_.openFromFile("../src/soundfiles/sandbox.wav")) {
                std::cerr << "Failed to load level background music!" << std::endl;
            } else {
                music_.setLoop(true);
                music_.setVolume(50);
                startMusic();
            }
        }

        void startMusic() override {
            music_.stop();
            music_.play();
        }

        void stopMusic() override {
            music_.stop();
        }

        std::pair<std::string, int> getReturn(std::string name) { return std::pair<std::string, int>(name, level_number_); }


        void playBinSound() { sound_.play(); }

        /**
         * @brief process the event that the user triggers and call the method according to it
         * 
         */
        std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) override {
            switch (event.type)
            {
                case sf::Event::MouseButtonPressed:     // mouse click
                {
                    processMouseButtonPress(event, window);
                    break;
                }
                case sf::Event::MouseMoved:             // mouse move
                {
                    processMouseMove(window);
                    break;
                }
                case sf::Event::MouseButtonReleased:    // click release
                {
                    std::string action_name;
                    action_name = processMouseButtonRelease(window);
                    if (action_name != "") { return getReturn(action_name); }
                    break;
                }
                case sf::Event::MouseWheelScrolled:     // zoom
                {
                    processMouseWheelScroll(event, window, view);
                    break;
                }
                default: { break; }
            }
            return getReturn("");
        }

        void processMouseButtonPress(const sf::Event& event, sf::RenderWindow& window) {
            if (event.mouseButton.button == sf::Mouse::Left) {  // left button click
                sf::Vector2i position = sf::Mouse::getPosition(window);
                sf::Vector2f globalPosition = window.mapPixelToCoords(position);
                // std::cout << "mouse position when pressed: " << globalPosition.x << " " << globalPosition.y << std::endl;
                // check if button
                for (auto& button : buttons_) {
                    if (button->inBounds(position, window)) {
                        clicked_ = true;
                        buttonClicked_ = button;
                        button->changeToDarkRed();
                        break;
                        //return button->getAction();
                    }
                }
                if (!object_in_turn_) {
                    for (auto& bird : birds_) {
                        if (bird->inBounds(position, window)) {
                            dragging_ = true;
                            object_in_turn_ = bird;
                            object_in_turn_->getBody()->SetAwake(false);
                            posOfBird_ = object_in_turn_->getBody()->GetPosition();
                        }
                    }
                    for (auto& pig : pigs_) {
                        if (pig->inBounds(position, window)) {
                            dragging_ = true;
                            object_in_turn_ = pig;
                            object_in_turn_->getBody()->SetAwake(false);
                        }
                    }
                    for (auto& obstacle : obstacles_) {
                        if (obstacle->inBounds(position, window)) {
                            dragging_ = true;
                            object_in_turn_ = obstacle;
                            object_in_turn_->getBody()->SetAwake(false);
                        }
                    }
                    if (star_) {
                        if (star_->inBounds(position, window)) {
                            dragging_ = true;
                            object_in_turn_ = star_;
                            object_in_turn_->getBody()->SetAwake(false);
                        }
                    }
                }   
            }
        }

        void processMouseMove(sf::RenderWindow& window) {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            sf::Vector2f globalPosition = window.mapPixelToCoords(position);
            if (object_in_turn_ && dragging_) {
                b2Vec2 newPos(globalPosition.x/100.0f, globalPosition.y/100.0f);
                object_in_turn_->getBody()->SetTransform(newPos, 0);
                if (bin_button_->inBounds(position, window)) {
                    bin_button_->changeToDarkRed();
                }
                else { bin_button_->changeToRed(); }
            }
            if (!clicked_ && !object_in_turn_) {
                for (auto& button : buttons_) {
                    if (button->inBounds(position, window)) {
                        button->changeToRed();
                    }
                    else {
                        button->changeToPurple();
                    }
                }
            }
        }

        std::string processMouseButtonRelease(sf::RenderWindow& window) {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            if (clicked_) {
                if (buttonClicked_->inBounds(position, window)) {
                    buttonClicked_->changeToPurple();
                    clicked_ = false;
                    std::pair<std::string, int> action = buttonClicked_->getAction();
                    if (action.first == "save") {
                        saveToFile();
                    }
                    else if (action.first == "menu" || action.first == "open") {
                        return action.first;
                    }
                    else { createObject(action.first); }
                }
                buttonClicked_->getShape().setFillColor(sf::Color::Magenta);
            }
            else if (object_in_turn_) {
                if (bin_button_->inBounds(position, window)) {
                    removeObject();
                }
                else {
                    if (!dynamic_cast<Star*>(object_in_turn_.get())) {
                        if (dynamic_cast<Bird*>(object_in_turn_.get())) {
                            object_in_turn_->getBody()->SetTransform(posOfBird_, 0);
                        }
                        object_in_turn_->getBody()->SetAwake(true);
                    }
                }
                dragging_ = false;
                object_in_turn_ = nullptr;
            }
            clicked_ = false;
            return "";
        }

        /**
         * @brief Create objects into the game
         *  
         */
        void createObject(std::string name) {
            if (name == "red") {
                std::shared_ptr<RedBird> bird = std::make_shared<RedBird>(40*birdcount_+130, 585);
                bird->initializePhysicsWorld(world_);
                birds_.push_back(bird);
                birdcount_++;
            }
            else if (name == "yellow") {
                std::shared_ptr<YellowBird> bird = std::make_shared<YellowBird>(40*birdcount_+130, 590);
                bird->initializePhysicsWorld(world_);
                birds_.push_back(bird);
                birdcount_++;
            }
            else if (name == "pig") {
                std::shared_ptr<NormalPig> pig = std::make_shared<NormalPig>(683, 0);
                pig->initializePhysicsWorld(world_);
                pigs_.push_back(pig);
            }
            else if (name == "king") {
                std::shared_ptr<KingPig> pig = std::make_shared<KingPig>(683, 0);
                pig->initializePhysicsWorld(world_);
                pigs_.push_back(pig);
            }
            else if (name == "wood") {
                std::shared_ptr<WoodObstacle> obstacle = std::make_shared<WoodObstacle>(683, 0);
                obstacle->initializePhysicsWorld(world_);
                obstacles_.push_back(obstacle);
            }
            else if (name == "stone") {
                std::shared_ptr<StoneObstacle> obstacle = std::make_shared<StoneObstacle>(683, 0);
                obstacle->initializePhysicsWorld(world_);
                obstacles_.push_back(obstacle);
            }
            else if (name == "glass") {
                std::shared_ptr<GlassObstacle> obstacle = std::make_shared<GlassObstacle>(683, 0);
                obstacle->initializePhysicsWorld(world_);
                obstacles_.push_back(obstacle);
            }
            else if (name == "star" && !star_) {
                star_ = std::make_shared<Star>(683, 384);
                star_->initializePhysicsWorld(world_);
                star_->getBody()->SetAwake(false);
            }
        }

        /**
         * @brief Save the player's level into a file so that it can be played later
         * 
         */
        void saveToFile() {
            highscores_.clearSandBoxScores();
            std::ofstream sandboxfile("../src/textfiles/sandboxlevel.txt");
            if (sandboxfile.is_open()) {
                // std::cout << "Sandbox file opened and cleared successfully" << std::endl;
                sandboxfile << "Birds\n";
                for (auto& bird : birds_) {
                    if (std::dynamic_pointer_cast<RedBird>(bird)) {
                        sandboxfile << "Red\n";
                    }
                    if (std::dynamic_pointer_cast<YellowBird>(bird)) {
                        sandboxfile << "Yellow\n";
                    }
                }
                sandboxfile << "\nPigs";
                for (auto& pig : pigs_) {
                    if (std::dynamic_pointer_cast<NormalPig>(pig)) {
                        sandboxfile << "\nNormal " << static_cast<int>(std::floor(pig->getX())) << " " << static_cast<int>(std::floor(pig->getY()));
                    }
                    if (std::dynamic_pointer_cast<KingPig>(pig)) {
                        sandboxfile << "\nKing " << static_cast<int>(std::floor(pig->getX())) << " " << static_cast<int>(std::floor(pig->getY()));
                    }
                }
                sandboxfile << "\n\nObstacles";
                for (auto& obstacle : obstacles_) {
                    if (std::dynamic_pointer_cast<WoodObstacle>(obstacle)) {
                        sandboxfile << "\nWood " << static_cast<int>(std::floor(obstacle->getX())) << " " << static_cast<int>(std::floor(obstacle->getY()));
                    }
                    else if (std::dynamic_pointer_cast<StoneObstacle>(obstacle)) {
                        sandboxfile << "\nStone " << static_cast<int>(std::floor(obstacle->getX())) << " " << static_cast<int>(std::floor(obstacle->getY()));
                    }
                    else if (std::dynamic_pointer_cast<GlassObstacle>(obstacle)) {
                        sandboxfile << "\nGlass " << static_cast<int>(std::floor(obstacle->getX())) << " " << static_cast<int>(std::floor(obstacle->getY()));
                    }
                }
                if (star_) {
                    sandboxfile << "\n\nStar\n";
                    sandboxfile << static_cast<int>(std::floor(star_->getX())) << " " << static_cast<int>(std::floor(star_->getY()));
                }
                // std::cout << "File written successfully" << std::endl;
            }
            else { std::cerr << "Failed to open the file.\n"; }
            saved_ = true;
            saveClock_.restart();
        }

        /**
         * @brief Remove an object from the level
         * 
         */
        void removeObject() {
            playBinSound();
            pigs_.erase(std::remove(pigs_.begin(), pigs_.end(), object_in_turn_), pigs_.end());
            obstacles_.erase(std::remove(obstacles_.begin(), obstacles_.end(), object_in_turn_), obstacles_.end());
            bodiesToRemove_.push_back(object_in_turn_->getBody());
            if (dynamic_cast<Star*>(object_in_turn_.get())) {
                star_ = nullptr;
            }
            if (dynamic_cast<Bird*>(object_in_turn_.get())) {
                // std::cout << "Before remove: ";
                for (const auto& bird : birds_) {
                    // std::cout << bird << " . "; 
                }
                // std::cout << std::endl;
                // std::cout << "removing bird" << std::endl;
                auto it1 = std::find(birds_.begin(), birds_.end(), object_in_turn_);
                size_t index = std::distance(birds_.begin(), it1);
                auto it = std::remove(birds_.begin(), birds_.end(), object_in_turn_);
                if (it != birds_.end()) {
                    birds_.erase(it);
                    // std::cout << index << std::endl;
                    for (size_t i = index; i < birds_.size(); ++i) {
                        b2Vec2 currentPos = birds_[i]->getBody()->GetPosition();
                        birds_[i]->getBody()->SetTransform(b2Vec2(currentPos.x-0.4, currentPos.y), 0);
                        // std::cout << "bird on index " << index << " moved" << std::endl;
                    }
                }
                birdcount_--;
                // std::cout << "After remove: ";
                for (const auto& bird : birds_) {
                    // std::cout << bird << " . ";
                }
                // std::cout << std::endl;
            }
        }

        /**
         * @brief Process zooming
         * 
         */
        void processMouseWheelScroll(const sf::Event& event, sf::RenderWindow& window, sf::View& view) {
            // Get the mouse position in window coordinates
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // Convert mouse position to world coordinates
            sf::Vector2f worldPosBeforeZoom = window.mapPixelToCoords(mousePos);
            // Zoom factor
            const float zoomFactor = 1.05f;
            if (event.mouseWheelScroll.delta < 0 && currentZoom_ < 13) {
                // Zoom in
                // std::cout << "in" << std::endl;
                view.zoom(1.0f / zoomFactor);
                currentZoom_++;
                // std::cout << currentZoom << std::endl;
            } else if (event.mouseWheelScroll.delta > 0 && currentZoom_ > -1) {
                // Zoom out
                // std::cout << "out" << std::endl;
                view.zoom(zoomFactor);
                currentZoom_--;
                // std::cout << currentZoom << std::endl;   
            }
            // Apply the updated view
            if (currentZoom_ != -1 && currentZoom_ != 13) {
                // Get the new world position of the mouse after zoom
                sf::Vector2f worldPosAfterZoom = window.mapPixelToCoords(mousePos, view);
                // Calculate the offset introduced by the zoom
                sf::Vector2f offset = worldPosBeforeZoom - worldPosAfterZoom;
                // Move the view's center by the offset
                view.move(offset);
                // Apply the updated view
                if (currentZoom_ == 0) {
                    view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                }
                window.setView(view);
            }
        }

        /**
         * @brief Make sure the position of the view is in the game bounds
         * 
         * @return the center of the view 
         */
        sf::Vector2f clampView(sf::View& view, sf::FloatRect& worldbounds) {
            sf::Vector2f viewSize = view.getSize();
            sf::Vector2f viewCenter = view.getCenter();

            float minX = worldbounds.left + viewSize.x/2.0f;
            float maxX = worldbounds.width - viewSize.x/2.0f;
            float minY = worldbounds.top + viewSize.y/2.0f;
            float maxY = worldbounds.height - viewSize.y/2.0f;

            viewCenter.x = std::clamp(viewCenter.x, minX, maxX);
            viewCenter.y = std::clamp(viewCenter.y, minY, maxY);

            view.setCenter(viewCenter);

            return sf::Vector2f(viewCenter.x, viewCenter.y);

        }

        // Calculates length of vector, for launching a bird
        double length(sf::Vector2i v) { return sqrt(v.x * v.x + v.y * v.y); }

        /**
         * @brief Update all the objects in the level (birds, pigs, obstacles, star)
         * 
         */
        std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) override {
            updatePhysics(deltaTime.asSeconds()); // Update the Box2D world

            // update object positions (SFML)
            for (auto bird : birds_) { bird->Update(); }
            for (auto pig : pigs_) { pig->Update(); }
            for (auto obstacle : obstacles_) { obstacle->Update(); }
            if (star_)
            {
                star_->Update();
            }
            

            return getReturn("");
        }

        /**
         * @brief Call the methods in the Render class to render the objects
         */
        void render(sf::RenderWindow& window, sf::View& view) override {
            window.clear();
            render_.renderBackground(window, "../src/imagefiles/level_background.png");
            // Add graphic objects
            render_.renderObstacle(window, *ground_);
            render_.renderSlingShot(window, slingshot_);
            if (star_)
            {
                render_.renderStar(window, *star_);
            }
            for (auto bird : birds_) { render_.renderBird(window, *bird); }
            for (auto pig : pigs_) { render_.renderPig(window, *pig); }
            for (auto obstacle : obstacles_) { render_.renderObstacle(window, *obstacle); }
            for (auto button : buttons_) { render_.renderButton(window, *button); }
            render_.renderButton(window, *bin_button_);
            if (saved_) {
                double elapsedTime = saveClock_.getElapsedTime().asSeconds();
                if (elapsedTime < 3)
                {
                    render_.renderHeading(window, "Level saved!", 120);
                }
                
                
                //render.renderButton(window, *menu_button_);
            }
            window.display();
        }

        /**
         * @brief Update the physics world of the player made level
         * 
         */
        void updatePhysics(double deltaTime) {
        
            double timeStep = 1.0f / 60.0f;
            physicsTime_ += deltaTime;

            while (physicsTime_ >= timeStep)
            {
                world_.Step(timeStep, 8, 3); // Updates the b2World by 1 "step"
                physicsTime_ -= timeStep;
            }
            for (b2Body* body : bodiesToRemove_) {
                Userdata* data = reinterpret_cast<Userdata*>(body->GetUserData().pointer);
                if (data)
                {
                    if (data->object)
                    {
                        // std::cout << data->objecttype << "data deleted" << std::endl;
                        delete data; // userdata object have object attributes that need to be freed.
                        world_.DestroyBody(body);
                        // std::cout << "body destroyed" << std::endl;
                    }
                }
            }
            bodiesToRemove_.clear();
        }

        bool isLevelState() override { return false; }

    private:
        b2Vec2 gravity_;
        b2World world_;
        std::shared_ptr<Object> object_in_turn_;
        Slingshot slingshot_;
        std::vector<std::shared_ptr<Button>> buttons_;
        std::vector<std::shared_ptr<Bird>> birds_;
        std::vector<std::shared_ptr<Pig>> pigs_;
        std::vector<std::shared_ptr<Obstacle>> obstacles_;
        std::shared_ptr<Ground> ground_;
        bool dragging_ = false;
        double physicsTime_ = 0;
        std::shared_ptr<Button> buttonClicked_;
        bool clicked_ = false;
        sf::Vector2i pressPosition_; // To store the position of mouse press
        sf::FloatRect worldbounds_ = sf::FloatRect(0, 0, 1366, 768);
        int birdcount_ = 0;
        HighScores highscores_;
        int level_number_ = 0;
        std::shared_ptr<Star> star_;
        bool saved_;
        //std::shared_ptr<Button> menu_button_;
        std::shared_ptr<Button> bin_button_;
        sf::Clock saveClock_; // clock to make the save heading appear for only a while
        std::vector<b2Body*> bodiesToRemove_;
        Render render_;
        sf::SoundBuffer soundBuffer_;
        sf::Sound sound_;
        sf::Music music_;
        b2Vec2 posOfBird_;
        int currentZoom_;
    };
