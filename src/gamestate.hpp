#pragma once

#include "render.hpp"
#include "leveldata.hpp"

#include <SFML/System/Time.hpp>

/**
 * @class GameState
 * @brief Abstract base class representing a game state.
 * 
 * Provides an interface for different states of the game, such as menu, gameplay, or game over.
 */
class GameState {
public:
    /**
     * @brief Default constructor for GameState.
     */
    GameState() {}

    /**
     * @brief Constructs a GameState with a player name.
     * 
     * @param name The player's name.
     */
    GameState(std::string name) : player_name_(name) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~GameState() = default;

    /**
     * @brief Process events for the game state.
     * 
     * Handles user input and other events specific to the state.
     * 
     * @param event The SFML event to process.
     * @param window The SFML render window.
     * @param view The current view of the game.
     * @return A pair containing the next action and associated level or state ID.
     */
    virtual std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) = 0;

    /**
     * @brief Update the game state.
     * 
     * Handles state-specific logic updates, such as animations or state transitions.
     * 
     * @param deltaTime The time elapsed since the last update.
     * @param window The SFML render window.
     * @param view The current view of the game.
     * @return A pair containing the next action and associated level or state ID.
     */
    virtual std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) = 0;

    /**
     * @brief Render the game state.
     * 
     * Draws the current state to the window.
     * 
     * @param window The SFML render window.
     * @param view The current view of the game.
     */
    virtual void render(sf::RenderWindow& window, sf::View& view) = 0;

    /**
     * @brief Determine if the current state is a level state.
     * 
     * @return Boolean value 'true' if it is a level state, 'false' otherwise.
     */
    virtual bool isLevelState() = 0;

    /**
     * @brief Get the width of the game window.
     * 
     * @return The width of the window in pixels.
     */
    int getWindowWidth () const {return window_width_; }

    /**
     * @brief Get the height of the game window.
     * 
     * @return The height of the window in pixels.
     */
    int getWindowHeight () const {return window_height_; }

    /**
     * @brief Get the player's name.
     * 
     * @return The name of the player.
     */
    std::string getPlayerName() { return player_name_; }

    /**
     * @brief Get the number of stars earned in the state.
     * 
     * @return The number of stars. Default is 0.
     */
    virtual int getStars() const { return 0; };

    /**
     * @brief Calculate the score for the current state.
     * 
     * @return The calculated score. Default is 0.
     */
    virtual int calculateScore() { return 0; };

    /**
     * @brief Start background music for the state.
     * 
     * This method can be overridden by derived classes to play specific music.
     */
    virtual void startMusic() {}

    /**
     * @brief Stop background music for the state.
     * 
     * This method can be overridden by derived classes to stop specific music.
     */
    virtual void stopMusic() {}

protected:
    int window_width_ = 1366;
    int window_height_ = 768;
    std::string player_name_;
};
