#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class Button
 * @brief Represents a graphical button that can be interacted with in a graphical user interface (GUI).
 * 
 * The Button class allows creating buttons that can display text or images, handle user input events,
 * and execute specific actions when clicked.
 */
class Button {
public:
    /**
     * @brief Constructs a new Button object.
     * 
     * @param x X-coordinate of the button's position.
     * @param y Y-coordinate of the button's position.
     * @param width Width of the button.
     * @param height Height of the button.
     * @param text Text or file path of the button's label or image.
     * @param action Action to perform when the button is clicked.
     * @param isImage True if the button uses an image instead of text.
     * @param isVolume True if the button represents a volume toggle.
     */
    Button(float x, float y, float width, float height, const std::string& text, std::pair<std::string,int> action, bool isImage, bool isVolume)
    : action_(action), clicked_(false), isVolume_(isVolume), isImage_(isImage) {
        shape_.setPosition(x, y);
        shape_.setSize(sf::Vector2f(width, height));
        shape_.setFillColor(sf::Color::Magenta);

        if (!isImage) {
            setFont(text, x, y, width, height);
        }
        else {
            setTexture(text, x, y, width, height);
        }
        
        loadTextures();
        if (!isVolume) {
            setNormalSprite(x, y, width, height);
        }   
        else {
            setVolumeSprite(x, y, width, height);
        }
        
    }

    /**
     * @brief Sets the button's text properties.
     * 
     * @param text Text to display on the button.
     * @param x X-coordinate of the button's position.
     * @param y Y-coordinate of the button's position.
     * @param width Width of the button.
     * @param height Height of the button.
     */
    void setFont(const std::string& text, float x, float y, float width, float height) {
        if (!font_.loadFromFile("../src/fontfiles/Lato-Regular.ttf")) {
                throw std::runtime_error("Could not load font from file");
            }
            text_.setFont(font_);
            text_.setString(text);
            text_.setFillColor(sf::Color::White);
            sf::FloatRect textBounds = text_.getLocalBounds();
            text_.setOrigin(textBounds.width / 2, textBounds.height / 2);
            text_.setPosition(x + width / 2, y + height / 2 - 5);
            text_.setOutlineColor(sf::Color(50, 50, 50, 128)); // dark grey color
            text_.setOutlineThickness(1);
    }

    /**
     * @brief Sets the button's texture properties using an image file.
     * 
     * @param text File path to the texture image.
     * @param x X-coordinate of the button's position.
     * @param y Y-coordinate of the button's position.
     * @param width Width of the button.
     * @param height Height of the button.
     */
    void setTexture(const std::string& text, float x, float y, float width, float height) {
        if (!texture_.loadFromFile(text)) {
                throw std::runtime_error("Could not load image from file");
            }
            sprite_.setTexture(texture_);
            sf::FloatRect bounds = sprite_.getLocalBounds();
            sprite_.setOrigin(bounds.width / 2, bounds.height / 2);

            float texture_x = static_cast<float>(texture_.getSize().x);
            float texture_y = static_cast<float>(texture_.getSize().y);
            float x_scale = (height - 10) / texture_x;
            float y_scale = (height - 10) / texture_y;

            sprite_.setScale(x_scale, y_scale);
            sprite_.setPosition(x + width / 2, y + height / 2);
    }

    /**
     * @brief Loads the textures used for the button's visual states.
     * 
     * This includes normal, clicked, and special states (e.g., volume or mute icons).
     */
    void loadTextures() {
        if (!purpleTexture_.loadFromFile("../src/imagefiles/purple.png")) {
            throw std::runtime_error("Could not load purple image from file");
        }
        if (!redTexture_.loadFromFile("../src/imagefiles/red.png")) {
            throw std::runtime_error("Could not load red image from file");
        }
        if (!darkRedTexture_.loadFromFile("../src/imagefiles/dark.png")) {
            throw std::runtime_error("Could not load dark red image from file");
        }
        if (!volumeTexture_.loadFromFile("../src/imagefiles/volumeIcon.png")) {
            throw std::runtime_error("Could not load dark volume image from file");
        }
        if (!muteTexture_.loadFromFile("../src/imagefiles/muteIcon.png")) {
            throw std::runtime_error("Could not load dark mute image from file");
        }
    }

    /**
     * @brief Sets the sprite for a normal button.
     * 
     * @param x X-coordinate of the button's position.
     * @param y Y-coordinate of the button's position.
     * @param width Width of the button.
     * @param height Height of the button.
     */
    void setNormalSprite(float x, float y, float width, float height) {
        buttonSprite_.setTexture(purpleTexture_);
        sf::FloatRect bounds = buttonSprite_.getLocalBounds();
        buttonSprite_.setOrigin(bounds.width / 2, bounds.height / 2);

        float texture_x = static_cast<float>(purpleTexture_.getSize().x);
        float texture_y = static_cast<float>(purpleTexture_.getSize().y);
        float x_scale = (width) / texture_x;
        float y_scale = (height) / texture_y;

        buttonSprite_.setScale(x_scale, y_scale);
        buttonSprite_.setPosition(x + width / 2, y + height / 2);
    }

    /**
     * @brief Sets the sprite for a volume control button.
     * 
     * @param x X-coordinate of the button's position.
     * @param y Y-coordinate of the button's position.
     * @param width Width of the button.
     * @param height Height of the button.
     */
    void setVolumeSprite(float x, float y, float width, float height) {
        // std::cout << "image" << std::endl;
        buttonSprite_.setTexture(volumeTexture_);
        sf::FloatRect bounds = buttonSprite_.getLocalBounds();
        buttonSprite_.setOrigin(bounds.width / 2, bounds.height / 2);

        float texture_x = static_cast<float>(volumeTexture_.getSize().x);
        float texture_y = static_cast<float>(volumeTexture_.getSize().y);
        float x_scale = (width) / texture_x;
        float y_scale = (height) / texture_y;

        buttonSprite_.setScale(x_scale, y_scale);
        buttonSprite_.setPosition(x + width / 2, y + height / 2);
    }

    /**
     * @brief Changes the button's texture to red.
     */
    void changeToRed() {
        buttonSprite_.setTexture(redTexture_);
    }

    /**
     * @brief Changes the button's texture to purple.
     */
    void changeToPurple() {
        buttonSprite_.setTexture(purpleTexture_);
    }

    /**
     * @brief Changes the button's texture to dark red.
     */
    void changeToDarkRed() {
        buttonSprite_.setTexture(darkRedTexture_);
    }

    /**
     * @brief Changes the button's texture to a volume icon.
     */
    void changeToVolume() {
        buttonSprite_.setTexture(volumeTexture_);
        volume_ = true;
    }

    /**
     * @brief Changes the button's texture to a mute icon.
     */
    void changeToMute() {
        buttonSprite_.setTexture(muteTexture_);
        volume_ = false;
    }

    /**
     * @brief Draws the button on the given window.
     * 
     * @param window The render window to draw the button on.
     */
    void draw(sf::RenderWindow& window) {
        window.draw(buttonSprite_);
        if (isImage_) {
            window.draw(sprite_);
        }
        else {
            window.draw(text_);
        }
    }

    /**
     * @brief Checks if a given position is within the button's bounds.
     * 
     * @param position Mouse position (the user has clicked) to check.
     * @param window The render window for coordinate transformation.
     * 
     * @return Boolean value 'true' if the position is within the button's bounds, 'false' otherwise.
     */
    bool inBounds(sf::Vector2i position, sf::RenderWindow& window) {
        sf::Vector2f globalPosition = window.mapPixelToCoords(position);
        sf::FloatRect bounds = buttonSprite_.getGlobalBounds();
        return bounds.contains(globalPosition);
    }

    /**
     * @brief Gets the sprite representing the button.
     * 
     * This sprite is used for rendering the button in the GUI and contains
     * the button's current visual representation (e.g., its texture).
     * 
     * @return A reference to the button's sprite.
     */
    sf::Sprite& getSprite() { return buttonSprite_; }

    /**
     * @brief Processes an SFML event to handle button interactions.
     * 
     * @param event The SFML event to process.
     * @param window The render window for mouse position transformation.
     * @param view The current view for mapping coordinates.
     * 
     * @return A pair representing the action to perform and an associated integer value.
     */
    std::pair<std::string, int> processEvent(const sf::Event& event, sf::RenderWindow& window, sf::View& view) {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {  // left button click
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position, window)) {
                    clicked_ = true;
                    if (!isVolume_) {
                        changeToDarkRed();
                    }
                    return std::pair<std::string, int>("click", 0);
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (clicked_) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position, window)) {
                    if (!isVolume_) { // if not volume button
                        changeToPurple();
                        return action_;
                    }
                    else { // if volume button
                        if (volume_) { // if volumeIcon is actice
                            changeToMute(); // change to mute icon
                            clicked_ = false; 
                            return std::pair<std::string, int>("mute", 0);
                        }
                        else {     // if muteIcon is active
                            changeToVolume(); // change to volumeIcon
                            clicked_ = false;
                            return std::pair<std::string, int>("volume", 0);
                        }
                    }
                }
                if (!isVolume_) {
                    changeToPurple();
                }
                clicked_ = false;
            }
            break;
        default:
            if (!clicked_) {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if (inBounds(position, window)) {
                    if (!isVolume_) {
                        changeToRed();
                    }
                }
                else {
                    if (!isVolume_) {
                        changeToPurple();
                    }
                }
            }
            break;
        }
        return std::pair<std::string, int>("", 0);
    }

    /**
     * @brief Checks if the volume is currently active.
     * 
     * @return Boolean value 'true' if the volume is active, 'false' otherwise.
     */
    bool isVolumeActive() { return volume_; }

    /**
     * @brief Gets the button's shape.
     * 
     * @return A reference to the button's shape object.
     */
    sf::RectangleShape& getShape() { return shape_; };

    /**
     * @brief Gets the button's associated action.
     * 
     * @return A pair representing the action and its associated integer value.
     */
    std::pair<std::string,int> getAction() { return action_; };


private:
    sf::Font font_;                         // Font used for text rendering
    sf::RectangleShape shape_;              // Shape of the button
    sf::Text text_;                         // Text displayed on the button
    std::pair<std::string, int> action_;    // Action to perform on button click
    
    sf::Sprite sprite_;                     // Sprite for image-based buttons
    sf::Sprite buttonSprite_;               // The main sprite representing the button

    sf::Texture texture_;                   // Texture for the button's image
    sf::Texture purpleTexture_;             // Texture for the button's normal state
    sf::Texture redTexture_;                // Texture for the button's highlighted state
    sf::Texture darkRedTexture_;            // Texture for the button's clicked state
    sf::Texture currenttexture_;            
    sf::Texture volumeTexture_;             // Texture for the volume icon
    sf::Texture muteTexture_;               // Texture for the mute icon
    
    bool isImage_;                          // True if the button uses an image instead of text
    bool clicked_;                          // True if the button is currently clicked
    bool isVolume_;                         // True if the button is a volume toggle
    bool volume_ = true;                    // True if the volume is active
};