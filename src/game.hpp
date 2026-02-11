#pragma once

#include <vector>
#include <string>
#include <stack>
#include <box2d/box2d.h>

// Include necessary header files
#include "leveldata.hpp"
#include "namestate.hpp"
#include "menustate.hpp"
#include "levelstate.hpp"
#include "gameoverstate.hpp"
#include "sandboxstate.hpp"
#include "render.hpp"
#include "collisiondetection.hpp"

/**
 * @class Game
 * @brief Takes care of game loop.
 *
 * The Game class is used to create and manage a game object, which controls game states.
 */

class Game {
public:

    /**
     * @brief Constructs a new Game object based on information saved on file.
     * If no textfile is provided, initializes empty matrix for highscores.
     * 
     * @param filename The name of the file to be read.
     */
    Game();

    /**
     * @brief Starts main game loop and handles stack.
     * 
     */
    void run();

    /**
     * @brief Push new state on state stack.
     * 
     * @param state State to be pushed.
     */
    void pushState(std::unique_ptr<GameState> state) {
        if (!states_.empty()) {
            states_.top()->stopMusic();
        }
        states_.push(std::move(state));
        states_.top()->startMusic();
    }

    /**
     * @brief Pop the top state from state stack.
     * 
     */
    void popState() {
        if (!states_.empty()) {
            states_.top()->stopMusic();
            states_.pop();
        }
        if (!states_.empty()) {
            states_.top()->startMusic();
        }
    }

    /**
     * @brief Changes the rop state on state stack.
     * First removes current top state in stack.
     * Then adds new state to top of the stack.
     * @param state The new top state in state stack.
     */
    void changeState(std::unique_ptr<GameState> state) {
        popState();
        pushState(std::move(state));
    }

private:
    std::vector<HighScores> highscores_;
    sf::RenderWindow window_;
    std::stack<std::unique_ptr<GameState>> states_;
    sf::View view_ = sf::View(sf::FloatRect(0, 0, 1366, 768));
    int currentZoom_ = 0;
    std::string player_name_;
};
