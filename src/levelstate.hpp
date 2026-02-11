#pragma once

#include "gamestate.hpp"
#include "leveldata.hpp"
#include "collisiondetection.hpp"
#include "highscores.hpp"
#include <cmath>
#include <memory>

    /**
     * @class LevelState
     * @brief Represents the state of a single level in the game.
     * 
     * This class is responsible for managing the game objects, such as birds, pigs, obstacles,
     * and the environment, within a single level. It handles the interaction between these objects
     * and the game's physics system. It also controls the level's state transitions,
     * such as win, lose, and restarting a level.
     */
    class LevelState : public GameState {
    public:
        /**
         * @brief Constructs a LevelState for a given level number.
         * 
         * Initializes the game objects for the level, including birds, pigs, obstacles, and ground.
         * It also sets up physics, music, and UI buttons
         * for restarting the level or returning to the main menu.
         * 
         * @param number The level number to initialize.
         */
        LevelState(int number) : level_number_(number), gravity_(0.0f, 9.8f), world_(gravity_), bird_in_turn_(nullptr), highscores_(HighScores().getHighScores(number)), score_(0), collisionListener_(clockForWorld_), level_empty_(false), currentZoom_(1) {

            LevelData data(number);
            initMusic();
            initButtons();
            for (auto& bird : data.getBirds()) {   
                bird->initializePhysicsWorld(world_); // adds bird to b2 world
                birds_.push_back(bird);
            }
            for (auto& pig : data.getPigs()) {
                pig->initializePhysicsWorld(world_); // adds pig to b2 world
                pigs_.push_back(pig);
            }
            for (auto& obstacle : data.getObstacles()) {
                obstacle->initializePhysicsWorld(world_); // adds obstacle to b2 world
                obstacles_.push_back(obstacle);
            }
            if (birds_.empty() || pigs_.empty()) {
                level_empty_ = true;
            }
            ground_ = data.getGround();
            ground_->initializePhysicsWorld(world_); // adds ground to b2 world
            ground_->setData(); // ground knows it is "ground"
            ground_->moveBodyDown(0.05f); // move ground body down 5 pixels so objects do not levitate
            star_ = data.getStar();
            if (star_) {
                star_->initializePhysicsWorld(world_);
                star_->setData();
                star_->setBodyStatic();
                star_->getBody()->GetFixtureList()->SetSensor(true);
            }
            // clockForWorld_.restart(); // Start the clock created to track time since creation of the b2 world
            world_.SetContactListener(&collisionListener_); // connect a self-made collisionlistener object to the b2 world
        }

        /**
         * @brief Destructor that stops the music when the level state is destroyed.
         */
        ~LevelState() {
            stopMusic();
        }

        /**
         * @brief Initializes the UI buttons for the level.
         * 
         * Sets the positions and labels of the buttons, such as "Restart", "Menu", and "Edit" (if the level is a sandbox level).
         */
        void initButtons() {
            int width = 130;
            int height = 55;
            int space = 20;
            int pos_x = getWindowWidth() - width - space;        // places button to right of window horizontally
            int pos_y = getWindowHeight() - height - space;       // places button to right of window vertically
            
            buttons_.push_back(std::make_shared<Button>(pos_x, pos_y, width, height, "Restart", std::pair<std::string,int>("restart", level_number_), false, false));
            buttons_.push_back(std::make_shared<Button>(pos_x, pos_y - height - space, width, height, "Menu", std::pair<std::string,int>("menu", 0), false, false));
            if (level_number_ == 4) {
                buttons_.push_back(std::make_shared<Button>(pos_x, pos_y - 2 * height - 2 * space, width, height, "Edit", std::pair<std::string,int>("sandbox", level_number_), false, false));
            }
        }

        /**
         * @brief Initializes the background music for the level.
         * 
         * Loads the music file and starts the music loop.
         */
        void initMusic() {
            if (!music_.openFromFile("../src/soundfiles/level.wav")) {
                std::cerr << "Failed to load level background music!" << std::endl;
            } else {
                music_.setLoop(true);
                music_.setVolume(25);
                startMusic();
            }
        }

        /**
         * @brief Starts the background music for the level.
         */
        void startMusic() override {
            music_.stop();
            music_.play();
        }

        /**
         * @brief Stops the background music for the level.
         */
        void stopMusic() override {
            music_.stop();
        }

        /**
         * @brief Retrieves the action associated with the level state.
         * 
         * @param name The action name.
         * @return A pair containing the action name and the level number.
         */
        std::pair<std::string, int> getReturn(std::string name) { return std::pair<std::string, int>(name, level_number_); }

        /**
         * @brief Calculates the number of stars the player earns based on performance.
         * 
         * @return The number of stars the player has earned.
         */
        int getStars() const override {
            if (star_) {
                int stars = 1;
                if (star_->getHp() == 0) { stars++; }
                if (score_ >= 5000) { stars++; }
                std::cout << "getStars() returns " << stars << "because score is " << score_ << " and starhp is " << star_->getHp() << std::endl;
                return stars;
            }
            return 0;
        };

        /**
         * @brief Calculates the score based on the current game state.
         * 
         * @return The updated score.
         */
        int calculateScore() override {
            score_ += collisionListener_.getScore(); 
            return score_;
        }

        /**
         * @brief Processes user input events such as mouse clicks and scrolls.
         * 
         * Handles the actions triggered by mouse button presses, mouse movements, and mouse wheel scrolls.
         * 
         * @param event The SFML event to process.
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         * @return A pair containing the result of the action.
         */
        std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) override {
            std::pair<std::string, int> action;
            for (auto button : buttons_) {
                action = button->processEvent(event, window, view);
                if (action == std::pair<std::string, int>("click", 0)) {
                    buttonClicked_ = button;
                    clicked_ = true;
                }
                else if (action != std::pair<std::string, int>("", 0)) { 
                    clicked_ = false;
                    return action;
                }
            }
            switch (event.type) {
                case sf::Event::MouseButtonPressed: {    // mouse click
                    processMouseButtonPress(event, window, view);
                    break;
                }
                case sf::Event::MouseMoved: {
                    processMouseMove(window, view);
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    std::string action = processMouseButtonRelease(window, view);
                    if (action != "") { return getReturn(action); }
                    break;
                }
                case sf::Event::MouseWheelScrolled: {
                    processWheelScroll(event, window, view);
                    break;
                }
                default: {
                    flyMotion(window, view);
                }
            }
            return getReturn("");
        }

        /**
         * @brief Handles mouse button press events during gameplay.
         * 
         * Updates the position of the bird in turn, triggers special actions, and manages the dragging of the bird.
         * 
         * @param event The SFML event that triggered the mouse button press.
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         */
        void processMouseButtonPress(const sf::Event& event, sf::RenderWindow& window, sf::View& view) {
            if (bird_in_turn_) {
                if (bird_in_turn_->isShot() && !bird_in_turn_->isFlying()) {
                    std::cout << "bird killed" << std::endl;
                    bird_in_turn_->kill(); 
                    bird_in_turn_ = nullptr;
                }
            }
            if (event.mouseButton.button == sf::Mouse::Left) {  // left button click
                sf::Vector2i position = sf::Mouse::getPosition(window);
                sf::Vector2f globalPosition = window.mapPixelToCoords(position);
                // std::cout << "mouse position when pressed: " << globalPosition.x << " " << globalPosition.y << std::endl;
                if (bird_in_turn_ && bird_in_turn_->isFlying() && !(bird_in_turn_->isSpecialActionUsed())) {
                    std::cout << "special action used" << std::endl;
                    bird_in_turn_->SpecialAction();
                }
                // if background clicked and there is no bird in turn, put next bird to slingshot
                else if ((!bird_in_turn_ || (bird_in_turn_->isSpecialActionUsed() && bird_in_turn_->isShot())) && !clicked_) {
                    view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                    clampView(view, worldbounds_);
                    currentZoom_ = 0;
                    window.setView(view);
                    for (auto& bird : birds_) {
                        if (!bird->isShot()) {
                            std::cout << "new bird set to in turn" << std::endl;
                            slingshot_.setBird(bird);
                            bird_in_turn_ = bird;
                            // std::cout << "sling: " << slingshot_.getX() << " " << slingshot_.getY() << std::endl;
                            b2Vec2 newPos(slingshot_.getX()/100.0f, slingshot_.getY()/100.0f);
                            bird_in_turn_->getBody()->SetTransform(newPos, 0);
                            break;
                        }
                    }
                }
                else if (bird_in_turn_ && !clicked_) {
                    sf::Vector2i distance(globalPosition.x-slingshot_.getX(), globalPosition.y-slingshot_.getY());
                    if (length(distance) > 70) {
                        distance.x = 70 * distance.x/length(distance);
                        distance.y = 70 * distance.y/length(distance);
                    }
                    b2Vec2 newPos((slingshot_.getX()+ distance.x)/100.0f, (slingshot_.getY() + distance.y)/100.0f);
                    bird_in_turn_->getBody()->SetTransform(newPos, 0);
                    dragging_ = true;
                    pressPosition_ = position;
                }
            }
            return;
        }

        /**
         * @brief Handles mouse movement during gameplay.
         * 
         * Updates the bird's position and the view's zoom level when the player is dragging the bird.
         * 
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         */
        void processMouseMove(sf::RenderWindow& window, sf::View& view) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f globalPos = window.mapPixelToCoords(mousePos);
            if (bird_in_turn_ && dragging_) {
                sf::Vector2i distance(globalPos.x - slingshot_.getX(), globalPos.y - slingshot_.getY());
                if (length(distance) > 70) {
                    distance.x = 70 * distance.x/length(distance);
                    distance.y = 70 * distance.y/length(distance);
                }
                b2Vec2 newPos((slingshot_.getX() + distance.x) / 100.0f, (slingshot_.getY() + distance.y) / 100.0f);
                bird_in_turn_->getBody()->SetTransform(newPos, 0);
                // zoom to bird when shooting
                view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                view.zoom(1.0f / (1.0f + length(distance) / 1000));
                currentZoom_ = 1000;
                // std::cout << "zoomikerroin: " << 1.0f / (1.0f + length(distance) / 1000) << std::endl;
                sf::Vector2f globalPosAfterZoom = window.mapPixelToCoords(mousePos, view);
                sf::Vector2f offset = globalPos - globalPosAfterZoom;
                view.move(offset);
                clampView(view, worldbounds_);
                window.setView(view);
            }
            return;
        }

        /**
         * @brief Handles mouse button release events during gameplay.
         * 
         * Shoots the bird in turn and stops the dragging action.
         * 
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         * @return An action string indicating the result of the release.
         */
        std::string processMouseButtonRelease(sf::RenderWindow& window, sf::View& view) {
            if (bird_in_turn_ && dragging_ && !clicked_) {
                //std::cout << position.x << " " << position.y << std::endl;
                std::cout << "bird shot" << std::endl;
                b2Vec2 impulse(-(bird_in_turn_->getX() - slingshot_.getX())/7.0f, -(bird_in_turn_->getY() - slingshot_.getY())/7.0f);
                bird_in_turn_->setBodyDynamic();
                bird_in_turn_->setVelocity(impulse);
                bird_in_turn_->speak();
                bird_in_turn_->fly();
                bird_in_turn_->shoot();
                dragging_ = false;
            }
            return "";
        }

        /**
         * @brief Controls the motion of the bird during flight.
         * 
         * Adjusts the camera view to follow the bird's movement during flight.
         * 
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         */
        void flyMotion(sf::RenderWindow& window, sf::View& view) {
            if (bird_in_turn_) {
                sf::Vector2f birdPos = bird_in_turn_->GetPosition();
                // sf::Vector2f viewPos = view.getCenter();
                float offset = birdPos.x - slingshot_.getX();
                view.move(offset / 1000, 0);
                clampView(view, worldbounds_);
                window.setView(view); 
            }
        }

        /**
         * @brief Handles mouse wheel scrolling events to zoom the view.
         * 
         * Allows the user to zoom in or out of the level using the mouse wheel.
         * 
         * @param event The SFML event that triggered the scroll.
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         */
        void processWheelScroll(const sf::Event& event, sf::RenderWindow& window, sf::View& view) {
            // Get the mouse position in window coordinates
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // Convert mouse position to world coordinates
            sf::Vector2f worldPosBeforeZoom = window.mapPixelToCoords(mousePos);
            // Zoom factor
            const float zoomFactor = 1.05f;
            if (currentZoom_ == 1000) {
                view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                clampView(view, worldbounds_);
                currentZoom_ = 0;
                std::cout << "zoom 0" << std::endl;
            }
            if (event.mouseWheelScroll.delta < 0 && currentZoom_ < 13) {
                // Zoom in
                // std::cout << "in" << std::endl;
                view.zoom(1.0f / zoomFactor);
                currentZoom_++;
                // std::cout << currentZoom << std::endl;
            } else if (event.mouseWheelScroll.delta > 0 && currentZoom_ > 0) {
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
                clampView(view, worldbounds_);
                // Apply the updated view
                if (currentZoom_ == 0) {
                    view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                }
                window.setView(view);
            }
            return;
        }

        /**
         * @brief Clamps the camera view within the boundaries of the level.
         * 
         * Ensures the camera stays within the specified world boundaries when the user moves the view.
         * 
         * @param view The SFML view to be clamped.
         * @param worldbounds The boundaries of the level.
         * @return The new center of the view.
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

        /**
         * @brief Calculates the length of a 2D vector, for launching a bird.
         * 
         * @param v The vector whose length is to be calculated.
         * @return The length of the vector.
         */
        double length(sf::Vector2i v) { return sqrt(v.x * v.x + v.y * v.y); }

        /**
         * @brief Updates the game state by processing physics and object positions.
         * 
         * Handles the physics simulation and updates the positions of the birds, pigs, obstacles, and other game objects.
         * 
         * @param deltaTime The elapsed time since the last update.
         * @param window The SFML render window.
         * @param view The SFML view to be updated.
         * @return A pair containing the action result.
         */
        std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) override {
            updatePhysics(deltaTime.asSeconds()); // Update the Box2D world
            flyMotion(window, view);
            // update object positions (SFML)
            for (auto bird : birds_) { bird->Update(); }
            for (auto pig : pigs_) { pig->Update(); }
            for (auto obstacle : obstacles_) { obstacle->Update(); }
            if (star_) { star_->Update(); }
            if (birdsAlive() == 0) {
                view.reset(sf::FloatRect(0.f, 0.f, 1366.f, 768.f));
                window.setView(view);
                sf::Clock clock;
                sf::Time seconds = sf::seconds(5); // The game updates 5 seconds after the launch of the last bird, then checks if won or not
                sf::Clock deltaClock;
                // Waiting 5 seconds before winning/losing
                while (clock.getElapsedTime() < seconds) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        switch (event.type) {
                            case sf::Event::MouseButtonPressed: {
                                if (bird_in_turn_) {
                                    if (bird_in_turn_->isFlying() && !(bird_in_turn_->isSpecialActionUsed())) {
                                        bird_in_turn_->SpecialAction();
                                    }
                                }
                            }
                            case sf::Event::MouseWheelScrolled: {
                                processWheelScroll(event, window, view);
                                break;
                            }
                            default: { break; }
                        }
                    }
                    sf::Time deltatime = deltaClock.restart();
                    updateWithOutWinCheck(deltatime);
                    render(window, view);
                }
                if (pigsAlive() > 0) { return getReturn("lose"); }
                else if (!level_empty_) { return getReturn("win"); }
                else { return getReturn(""); }
            }
            if (pigsAlive() == 0) {
                sf::Clock clock;
                sf::Time seconds = sf::seconds(3); // The game updates 3 seconds after the launch of the last bird, then checks if won or not

                sf::Clock deltaClock;
                // Waiting 3 seconds before winning/losing
                while (clock.getElapsedTime() < seconds) {
                    sf::Event event;
                    while (window.pollEvent(event)) {
                        switch (event.type) {
                            case sf::Event::MouseWheelScrolled: {
                                processWheelScroll(event, window, view);
                                break;
                            }
                            default: { break; }
                        }
                    }
                    sf::Time deltatime = deltaClock.restart();
                    updateWithOutWinCheck(deltatime);
                    render(window, view);
                }
                // check how many birds are left after a win and update points
                score_ += 5000 * birdsAlive();
                // std::cout << "score is now " << score_ + collisionListener_.getScore() << std::endl;
                if (!level_empty_) { return getReturn("win"); }
                else { return getReturn(""); }
            }
            return getReturn("");
        }

         /**
         * @brief Updates the game state without checking for win conditions.
         * 
         * Updates the physics and object positions without checking if the level is won or lost.
         * 
         * @param deltaTime The elapsed time since the last update.
         */
        void updateWithOutWinCheck(sf::Time deltaTime) {
            updatePhysics(deltaTime.asSeconds()); // Update the Box2D world
            // update object positions (SFML)
            for (auto bird : birds_) { bird->Update(); }
            for (auto pig : pigs_) { pig->Update(); }
            for (auto obstacle : obstacles_) { obstacle->Update(); }
            if (star_) { star_->Update(); }
        }

        /**
         * @brief Renders the game objects and UI to the screen.
         * 
         * Draws the background, obstacles, birds, pigs, buttons, and other UI elements to the window.
         * 
         * @param window The SFML render window.
         * @param view The SFML view to render with.
         */
        void render(sf::RenderWindow& window, sf::View& view) override {
            window.clear();
            render_.renderBackground(window, true);
            // Add graphic objects
            render_.renderObstacle(window, *ground_);
            render_.renderSlingShot(window, slingshot_);
            if (star_) { render_.renderStar(window, *star_); }
            // jotenkin calculatee score
            int currentscore = collisionListener_.getScore();
            render_.renderLevelInfo(window, pigsAlive(), birdsAlive(), currentscore);
            for (auto bird : birds_) { render_.renderBird(window, *bird); }
            for (auto pig : pigs_) { render_.renderPig(window, *pig); }
            for (auto obstacle : obstacles_) { render_.renderObstacle(window, *obstacle); }
            for (auto button : buttons_) { render_.renderButton(window, *button); }
            if (birds_.empty() || pigs_.empty()) {
                render_.renderHeading(window, "Create your level in Sandbox!", 120);
            }
            window.display();
        }

        /**
         * @brief Checks how many pigs are alive in the level.
         * 
         * @returns Integer number of pigs that are alive.
         */
        int pigsAlive() {
            int sum = 0;
            for (const auto& pig : pigs_) {
                if (pig->getHp() > 0) {
                    sum++;
                }
            }
            return sum;
        }

        /**
         * @brief Checks how many birds are alive in the level.
         * 
         * @returns Integer number of unused birds. 
         */
        int birdsAlive() {
            int sum = 0;
            for (const auto& bird : birds_) {
                if (!(bird->isShot())) {
                    sum++;
                }
            }
            return sum;
        }

        /**
         * @brief Updates the physics simulation of the level.
         * 
         * Advances the physics simulation of the Box2D world.
         * 
         * @param deltaTime The elapsed time since the last update.
         */
        void updatePhysics(double deltaTime) {
        
            double timeStep = 1.0f / 60.0f;
            physicsTime_ += deltaTime;

            while (physicsTime_ >= timeStep) {
                world_.Step(timeStep, 8, 3); // Updates the b2World by 1 "step"
                physicsTime_ -= timeStep;
            }
            
            // remove destroyed objects from the b2 world
            const auto& bodiesToRemove = collisionListener_.getBodiesToRemove();
            for (b2Body* body : bodiesToRemove) {
                Userdata* data = reinterpret_cast<Userdata*>(body->GetUserData().pointer);
                if (data) {
                    if (data->object) {
                        // std::cout << data->objecttype << "data deleted" << std::endl;
                        delete data; // userdata object have object attributes that need to be freed.
                        world_.DestroyBody(body);
                        // std::cout << "body destroyed" << std::endl;
                    }
                }
            }
            collisionListener_.clearBodiesToRemove();
        }

        /**
         * @brief Checks if this is a LevelState.
         * 
         * @return Always returns boolean value 'true' for LevelState.
         */
        bool isLevelState() override { return true; }

    private:
        int level_number_;
        b2Vec2 gravity_;
        b2World world_;
        std::shared_ptr<Bird> bird_in_turn_;
        Scores highscores_;
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
        int score_;
        sf::FloatRect worldbounds_ = sf::FloatRect(0, 0, 1366, 768);
        std::shared_ptr<Star> star_;
        CollisionListener collisionListener_;
        sf::Clock clockForWorld_;
        bool level_empty_;
        Render render_;
        int currentZoom_;
        sf::Music music_;
    };
