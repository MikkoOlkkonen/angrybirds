#pragma once

#include "gamestate.hpp"
#include "inputbox.hpp"

/**
 * @class NameState
 * @brief The state for inputting the player's name.
 *
 * The NameState class is responsible for handling the state where the user inputs their name.
 * It contains an input box where the player can type their name and a button to save the name.
 * The state transitions or performs actions based on the player's input.
 */
class NameState : public GameState {
public:
    /**
     * @brief Default constructor that initializes the input elements (input box and save button).
     */
    NameState() {
        initElems();
    }

    /**
     * @brief Initializes the input elements (input box and save button).
     * 
     * Sets up the input box and button in the center of the window and places the button to the right
     * of the input box.
     */
    void initElems() {
        float width = 300;
        float height = 50;
        float x = window_width_ / 2 - width / 2;
        float y = window_height_ / 2 - height / 2;
        inputbox_ = std::make_shared<InputBox>(x, y, width, height);
        button_ = std::make_shared<Button>(x + width + 20, y, 100, height, "Save", std::pair<std::string,int>("savename", 0), false, false);
    }

    /**
     * @brief Returns a pair containing the action name and associated value.
     * 
     * @param name The action name.
     * @return A pair of strings and integers representing the action and its associated value.
     */
    std::pair<std::string, int> getReturn(std::string name) { return std::pair<std::string, int>(name, 0); }

    /**
     * @brief Processes the user's input events.
     *
     * Handles button clicks and key press events. When the Enter key is pressed, it changes the button's
     * color to purple and triggers the button's action.
     *
     * @param event The event to process (e.g., mouse click or key press).
     * @param window The SFML window to check for events.
     * @param view The SFML view to manage coordinate transformations.
     * @return A pair containing the action name and value, indicating the next action to perform.
     */
    std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) override {
        std::pair<std::string, int> action;
        action = button_->processEvent(event, window, view);
        player_name_ = inputbox_->processEvent(event, window);
        if (action != std::pair<std::string, int>("", 0) && action != std::pair<std::string, int>("click", 0)) { 
            return action; 
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            std::cout << "should turn to purple now" << std::endl;
            button_->changeToPurple();
            render(window, view);
            return button_->getAction();
        }
        return getReturn("");
    }

    /**
     * @brief Renders the NameState to the screen.
     * 
     * Clears the window and renders the background, input box, and save button.
     * 
     * @param window The SFML window to render to.
     * @param view The SFML view to render the content.
     */
    void render(sf::RenderWindow& window, sf::View& view) override {
        window.clear();
        render_.renderBackground(window, false);
        render_.renderInputBox(window, *inputbox_);
        render_.renderButton(window, *button_);
        window.display();
    }

    /**
     * @brief Updates the NameState.
     * 
     * This function is not used in this state but is required for the interface.
     * 
     * @param deltaTime The time elapsed since the last frame.
     * @param window The SFML window to update.
     * @param view The SFML view to update.
     * @return A pair representing the action name and value.
     */
    std::pair<std::string, int> update(sf::Time deltaTime, sf::RenderWindow& window, sf::View& view) override { return getReturn(""); }

    /**
     * @brief Checks if the current state is a level state.
     * 
     * @return Boolean value 'false', since this is the name input state, not a level state.
     */
    bool isLevelState() override { return false; }

private:
    std::shared_ptr<InputBox> inputbox_;    // The input box for entering the player's name
    std::shared_ptr<Button> button_;        // The button to save the player's name
    Render render_;                         // The render object for drawing the background and UI elements
};