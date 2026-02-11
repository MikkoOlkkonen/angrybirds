#pragma once

#include "gamestate.hpp"
#include "highscores.hpp"

/**
 * @class GameOverState
 * @brief Represents the state of the game when it is over, either won or lost.
 */
class GameOverState : public GameState {
public:
    /**
     * @brief Construct a new GameOverState object.
     * 
     * @param level The level number.
     * @param win Boolean value whether the game was won (true) or lost (false).
     * @param score The score achieved by the player.
     * @param name The player's name.
     * @param stars The number of stars earned by the player.
     */
    GameOverState (int level, bool win, int score, std::string name, int stars) : GameState(name), highscores_(std::make_shared<HighScores>()), win_(win), score_(score), level_(level), stars_(stars) {
        initButtons();
        if (win_) {
            highscores_->insertNew(std::pair<int, std::string>(score_, getPlayerName()), level_);
        }
    }
    
    /**
     * @brief Initialize the buttons for the Game Over screen.
     */
    void initButtons() {
        int width = 200;
        int height = 75;
        int middle_h = getWindowWidth() / 2 - width / 2;        // places button centre in middle of window horizontally
        int pos_v = getWindowHeight() - 1.5 * height;           // places button centre bottomish of window
        
        buttons_.push_back(std::make_shared<Button>(middle_h - 1.5 * width, pos_v, width, height, "Restart Level", std::pair<std::string,int>("restart", level_), false, false));
        buttons_.push_back(std::make_shared<Button>(middle_h, pos_v, width, height, "Back to Menu", std::pair<std::string,int>("menu", 0), false, false));
        if (level_ < 3) {
            buttons_.push_back(std::make_shared<Button>(middle_h + 1.5 * width, pos_v, width, height, "Next Level", std::pair<std::string,int>("next", level_ + 1), false, false));
        }
    }

    /**
     * @brief Get a return action with a name and level.
     * 
     * @param name The name of the action.
     * @param level The level associated with the action.
     * @return A pair containing the name and level.
     */
    std::pair<std::string, int> getReturn(std::string name, int level) { return std::pair<std::string, int>(name, level); }

    /**
     * @brief Process user input events for the Game Over screen.
     * 
     * @param event The event to process.
     * @param window The render window.
     * @param view The view of the game.
     * @return A pair containing the action name and associated level.
     */
    std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) override {
        std::pair<std::string, int> action;
        for (auto button : buttons_) {
            action = button->processEvent(event, window, view);
            if (action != std::pair<std::string, int>("", 0) && action != std::pair<std::string, int>("click", 0)) { 
                return action; 
            }
        }
        return getReturn("", 0);
    }

    /**
     * @brief Update the Game Over state.
     * 
     * Plays win/lose sound effects and updates the state logic.
     * 
     * @param deltaTime The time elapsed since the last update.
     * @param window The render window.
     * @param view The view of the game.
     * @return A pair containing the action name and associated level.
     */
    std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) override { 
        if (!win_ && !soundPlayed_) {
            if (soundBuffer_.loadFromFile("../src/soundfiles/lose.wav")) {
                sound_.setBuffer(soundBuffer_);
                sound_.play();
                soundPlayed_ = true;
            } else {
                std::cerr << "Failed to load lose sound file!" << std::endl;
            }
        }
        else if (win_ && !soundPlayed_) {
            if (soundBuffer_.loadFromFile("../src/soundfiles/win.wav")) {
                sound_.setBuffer(soundBuffer_);
                sound_.play();
                soundPlayed_ = true;
            } else {
                std::cerr << "Failed to load win sound file!" << std::endl;
            }
        }
        return getReturn("", level_); 
    }

    /**
     * @brief Determine if this state is a level state.
     * 
     * @return Boolean value 'false', as GameOverState is not a level state.
     */
    bool isLevelState() override { return false; }

    /**
     * @brief Render the Game Over screen.
     * 
     * Displays background, heading, stars, high scores, and buttons.
     * 
     * @param window The render window.
     * @param view The view of the game.
     */
    void render(sf::RenderWindow& window, sf::View& view) override {
        window.clear();
        render_.renderBackground(window, false);
        if (win_) {
            render_.renderHeading(window, getPlayerName() + ", you won with score " + std::to_string(score_) + "!", 50);
            render_.renderStars(window, stars_);
            if (level_ < 4)
            {
                render_.renderHighScores(window, highscores_->getHighScores(level_));
            }
        }
        else { render_.renderHeading(window, getPlayerName() + ", you lost!", 50); }
        for (auto button : buttons_) {
            render_.renderButton(window, *button);
        }
        window.display();
    }

private:
    std::vector<std::shared_ptr<Button>> buttons_;  // Buttons displayed on the Game Over screen
    std::shared_ptr<HighScores> highscores_;        // High scores manager for the game
    std::shared_ptr<Button> buttonClicked_;         // Button that was clicked by the player
    
    bool win_;                                      // Flag to indicate if the game was won
    int score_;                                     // Player's score in the game
    int level_;                                     // The level number
    int stars_;                                     // The number of stars earned by the player

    sf::SoundBuffer soundBuffer_;                   // Sound buffer for win/lose sound effects
    sf::Sound sound_;                               // Sound object to play win/lose sound effects
    bool soundPlayed_ = false;                      // Flag to indicate if the win/lose sound has been played

    Render render_;                                 // Rendering helper for displaying UI components
};