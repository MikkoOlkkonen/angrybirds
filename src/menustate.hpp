#pragma once

#include "gamestate.hpp"
#include "inputbox.hpp"

/**
 * @class MenuState
 * @brief The state for the menu screen, which includes handling buttons for level selection, 
 *        sandbox mode, and volume control.
 *
 * The MenuState class inherits from GameState and manages the menu's UI, music, and event processing.
 * It handles the initialization of buttons, background music, and the volume control slider.
 */
class MenuState : public GameState {
public:
    /**
     * @brief Default constructor that initializes buttons and music.
     */
    MenuState() { 
        initButtons();
        initMusic();
    }
    
    /**
     * @brief Initializes the buttons for level selection and volume control.
     *
     * Creates buttons for selecting different levels, opening the sandbox mode, and adjusting the volume.
     * The buttons are positioned based on the window's size.
     */
    void initButtons() {
        int width = 175;
        int height = 75;
        int middle_h = getWindowWidth() / 2 - width / 2;                  // places button centre in middle of left side of window horizontally
        int middle_v = getWindowHeight() / 2;       // places button centre in middle of window vertically
        
        buttons_.push_back(std::make_shared<Button>(middle_h - width , middle_v - 1.5 * height, width, height, "Level 1", std::pair<std::string,int>("open", 1), false, false));
        buttons_.push_back(std::make_shared<Button>(middle_h - width, middle_v, width, height, "Level 2", std::pair<std::string,int>("open", 2), false, false));
        buttons_.push_back(std::make_shared<Button>(middle_h - width, middle_v + 1.5 * height, width, height, "Level 3", std::pair<std::string,int>("open", 3), false, false));
        buttons_.push_back(std::make_shared<Button>(middle_h + width, middle_v - 0.75 * height, width, height, "My Level", std::pair<std::string,int>("open", 4), false, false));
        buttons_.push_back(std::make_shared<Button>(middle_h + width, middle_v + 0.75 * height, width, height, "Sandbox", std::pair<std::string,int>("sandbox", 0), false, false));
        
        buttons_.push_back(std::make_shared<Button>(10, 10, 50, 50, "", std::pair<std::string,int>("volume", 0), false, true)); //VOlume button

        volumeSlide_ = std::make_shared<Button>(10, 85, 200, 5, "", std::pair<std::string,int>("volume", 0), false, false);
        volumeKnob_ = std::make_shared<Button>(102, 79, 16, 16, "", std::pair<std::string,int>("volume", 0), false, false);
    }   

    /**
     * @brief Destructor that stops the music when the menu state is destroyed.
     */
    ~MenuState() {
        stopMusic();
    }

    /**
     * @brief Initializes the background music for the menu.
     *
     * Loads the music file and sets it to loop.
     * If the music fails to load, an error is printed to the console.
     */
    void initMusic() {
        if (!music_.openFromFile("../src/soundfiles/menu.wav")) {
            std::cerr << "Failed to load menu background music!" << std::endl;
        } else {
            music_.setLoop(true);
            music_.setVolume(50);
            startMusic();
        }
    }

    /**
     * @brief Starts playing the background music.
     */
    void startMusic() override {
        music_.stop();
        music_.play();
    }

    /**
     * @brief Stops playing the background music.
     */
    void stopMusic() override {
        music_.stop();
    }

    /**
     * @brief Returns a pair containing the action name and associated level.
     * 
     * @param name The action name.
     * @param level The associated level for the action (e.g., 1, 2, 3).
     * @return A pair of strings and integers representing the action and level.
     */
    std::pair<std::string, int> getReturn(std::string name, int level) { return std::pair<std::string, int>(name, level); }

    /**
     * @brief Returns the list of buttons in the menu.
     * 
     * @return A vector of shared pointers to the Button objects.
     */
    std::vector<std::shared_ptr<Button>> getButtons() { return buttons_; }

    /**
     * @brief Processes user input events like mouse clicks and movements.
     *
     * Handles mouse button events to interact with the menu buttons and volume slider.
     * Adjusts the volume if the volume slider is being moved, and returns the action associated
     * with the button the user clicked.
     * 
     * @param event The event to process.
     * @param window The SFML window to check for events.
     * @param view The SFML view to manage coordinate transformations.
     * @return A pair of action name and level, indicating the user’s choice.
     */
    std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) override {
        std::pair<std::string, int> action;
        for (auto button : buttons_) {
            action = button->processEvent(event, window, view);
            if (action == std::pair<std::string, int>("volume", 0))
            {
                music_.setVolume(50);
                sf::Vector2f currentPos = volumeKnob_->getSprite().getPosition();
                volumeKnob_->getSprite().setPosition(102, currentPos.y);
            }
            else if (action == std::pair<std::string, int>("mute", 0))
            {
                music_.setVolume(0);
                sf::Vector2f currentPos = volumeKnob_->getSprite().getPosition();
                volumeKnob_->getSprite().setPosition(10, currentPos.y);
            }
            else if (action != std::pair<std::string, int>("", 0) && action != std::pair<std::string, int>("click", 0)) { 
                return action; 
            }
        }
        switch (event.type)
        {
            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (volumeKnob_->inBounds(position, window))
                {
                    sliding_ = true;
                }
                break;
            }
            case sf::Event::MouseMoved:
            {
                if (sliding_)
                {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    sf::Vector2f globalPosition = window.mapPixelToCoords(position);
                    sf::FloatRect slideBounds = volumeSlide_->getSprite().getGlobalBounds();
                    float newX = std::clamp(globalPosition.x, slideBounds.left, slideBounds.left + slideBounds.width);
                    volumeKnob_->getSprite().setPosition(newX, volumeKnob_->getSprite().getPosition().y);

                    float newVol = (volumeKnob_->getSprite().getPosition().x - 10)/2.0f;
                    
                    if (newVol == 0)
                    {
                        buttons_[buttons_.size()-1]->changeToMute();
                    }
                    else if (!(buttons_[buttons_.size()-1]->isVolumeActive()))
                    {
                        buttons_[buttons_.size()-1]->changeToVolume();
                    }
                    
                    
                    
                    music_.setVolume(newVol);
                }
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                sliding_ = false;
                break;
            }
            default:
            {
                break;
            }
        }
 
        return getReturn("", 0);
    }

    /**
     * @brief Updates the menu state.
     * 
     * This function is called every frame but does not perform any specific logic for the menu state.
     * 
     * @param deltaTime The time elapsed since the last frame.
     * @param window The SFML window to update.
     * @param view The SFML view to update.
     * @return A pair representing the action and level.
     */
    std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) override { return getReturn("", 0); }

    /**
     * @brief Checks if the current state is a level state.
     * 
     * @return Boolean value 'false', since this is the menu state, not a level state.
     */
    bool isLevelState() override { return false; }

    /**
     * @brief Renders the menu state to the screen.
     * 
     * Clears the window and renders the background, heading, and buttons (including the volume slider and knob).
     * 
     * @param window The SFML window to render to.
     * @param view The SFML view to render the content.
     */
    void render(sf::RenderWindow& window, sf::View& view) override {
        window.clear();     
        render_.renderBackground(window, false);
        render_.renderHeading(window, "Menu", 80);
        for (auto button : buttons_) {
            render_.renderButton(window, *button);
        }
        render_.renderButton(window, *volumeSlide_);
        render_.renderButton(window, *volumeKnob_);

        window.display();
    }

private:
    std::vector<std::shared_ptr<Button>> buttons_;
    std::shared_ptr<Button> buttonClicked_;
    Render render_;    // The render object for drawing the background and buttons

    std::shared_ptr<Button> volumeIconButton_;
    sf::Music music_;
    std::shared_ptr<Button> volumeSlide_;
    std::shared_ptr<Button> volumeKnob_;
    bool sliding_ = false;     // A flag indicating if the volume knob is being dragged
};