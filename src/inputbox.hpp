#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class InputBox
 * @brief A class for creating and managing text input fields in SFML.
 * 
 * The InputBox class handles user interactions, text input, and rendering for input fields.
 * It supports user text entry, mouse interactions, and basic styling.
 */
class InputBox {
public:
    /**
     * @brief Constructs an InputBox with specified dimensions and position.
     * 
     * @param x The x-coordinate of the input box in pixels.
     * @param y The y-coordinate of the input box in pixels.
     * @param width The width of the input box in pixels.
     * @param height The height of the input box in pixels.
     * @throws std::runtime_error if the font file cannot be loaded.
     */
    InputBox (float x, float y, float width, float height) : clicked_(false), can_write_(false) {
        outline_.setSize({width, height});
        outline_.setPosition(x, y); 
        outline_.setOutlineColor(sf::Color::Magenta);
        outline_.setOutlineThickness(2);

        if (!font_.loadFromFile("../src/fontfiles/Lato-Regular.ttf")) {
            throw std::runtime_error("Could not load font from file");
        }
        text_.setFont(font_);
        text_.setFillColor(sf::Color::Magenta);
        text_.setPosition(x + 5, y + 5);
        text_.setString(input_);

        prompt_.setFont(font_);
        prompt_.setFillColor(sf::Color(191, 191, 191));
        prompt_.setPosition(x + 5, y + 5);
        prompt_.setString("input nickname");
    }

    /**
     * @brief Gets the outline shape of the input box.
     * 
     * @return A reference to the SFML RectangleShape representing the input box's outline.
     */
    sf::RectangleShape& getOutline() { return outline_; }

    /**
     * @brief Gets the text currently displayed in the input box.
     * 
     * @return The SFML Text object containing the user's input.
     */
    sf::Text getText() { return text_; }

    /**
     * @brief Gets the prompt text displayed when the input box is empty.
     * 
     * @return The SFML Text object representing the prompt.
     */
    sf::Text getPrompt() const { return prompt_; };

    /**
     * @brief Checks if a given position is within the bounds of the input box.
     * 
     * @param position The mouse position in window coordinates.
     * @return Boolean value 'true' if the position is within the input box's bounds, 'false' otherwise.
     */
    bool inBounds(sf::Vector2i position) {
        sf::FloatRect bounds = outline_.getGlobalBounds();    // bounds
        return bounds.contains(static_cast<sf::Vector2f>(position));  // check if clicked position inside bounds 
    }

    /**
     * @brief Processes keyboard input and updates the text in the input box.
     * 
     * @param event The SFML event containing user input data.
     * @return The updated string input after processing the event.
     */
    std::string processInput(sf::Event event) {
        if (event.text.unicode == '\b') {
            if (!input_.empty()) {
                input_.pop_back();
                text_.setString(input_);
            }
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            input_ += static_cast<char>(event.text.unicode);
            text_.setString(input_);
        }
        return input_;
    }

    /**
     * @brief Checks if the input box contains any user-entered text.
     * 
     * @return Boolean value 'true' if the input box contains text, 'false' otherwise.
     */
    bool hasText() const {
        if (input_.empty()) { return false; }
        return true;
    }

    /**
     * @brief Processes various events to handle user interactions with the input box.
     * 
     * Handles mouse clicks, text input, and other relevant events. Updates the appearance
     * and state of the input box based on user interactions.
     * 
     * @param event The SFML event to process.
     * @param window The SFML window where the input box is displayed.
     * @return The updated string input after processing the event.
     */
    std::string processEvent(const sf::Event& event, sf::RenderWindow& window) {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {  // left button click
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position)) {
                    clicked_ = true;
                    can_write_ = true;
                    outline_.setOutlineColor(sf::Color(220, 0, 0, 255));
                }
                else { can_write_ = false; }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (clicked_) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position)) {
                    outline_.setOutlineColor(sf::Color::Red);
                }
                clicked_ = false;
            }
            break;
        case sf::Event::TextEntered:
            can_write_ = true;
            outline_.setOutlineColor(sf::Color::Red);
            return processInput(event);
        default:
            if (!clicked_) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position)) {
                    outline_.setOutlineColor(sf::Color::Red);
                }
                else if (!can_write_) {
                    outline_.setOutlineColor(sf::Color::Magenta);
                }
            }
            break;
        }
        return input_;
    }

private:
    sf::RectangleShape outline_;    // The graphical outline of the input box
    sf::Text text_;                 // The text entered by the user
    sf::Font font_;                 // The font used for rendering text
    std::string input_;             // The string representing the user's input
    sf::Text prompt_;               // The prompt text displayed when input is empty
    bool clicked_;                  // Flag to track whether the input box is currently clicked
    bool can_write_;                // Flag to indicate whether the user can write in the input box
    std::string player_name_;
};