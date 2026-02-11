#pragma once

#include "button.hpp"
#include "pig.hpp"
#include "bird.hpp"
#include "obstacle.hpp"
#include "highscores.hpp"
#include "slingshot.hpp"
#include "inputbox.hpp"
#include "star.hpp"


class Render {
    public:
        Render() {
            if (!starTexture_.loadFromFile("../src/imagefiles/star.png")) {
                throw std::runtime_error("Loading star failed!");
            }
            if (!starOutlineTexture_.loadFromFile("../src/imagefiles/star_outline.png")) {
                throw std::runtime_error("Loading star outline failed!");
            }
            if (!latoRegular_.loadFromFile("../src/fontfiles/Lato-Regular.ttf")) {
                throw std::runtime_error("Could not load font from file");
            }
            if (!latoBold_.loadFromFile("../src/fontfiles/Lato-Bold.ttf")) {
                throw std::runtime_error("Could not load font from file");
            }
            if (!latoBlack_.loadFromFile("../src/fontfiles/Lato-Black.ttf")) {
                throw std::runtime_error("Could not load font from file");
            }
            if (!levelBackgroundTexture_.loadFromFile("../src/imagefiles/level_background.png")) {
                throw std::runtime_error("Loading background failed!");
            }
            if (!menuBackgroundTexture_.loadFromFile("../src/imagefiles/menu_background.png")) {
                throw std::runtime_error("Loading background failed!");
            }
        }
        /**
         * @brief Draw Button object button.
         * 
         * @param button The Button object to be drawn.
         */
        void renderButton(sf::RenderWindow& window, Button& button) {
            button.draw(window);
        }

        /**
         * @brief Draw Pig object pig.
         * 
         * @param pig The Pig object to be drawn.
         */
        void renderPig(sf::RenderWindow& window, Pig& pig) {
            if (pig.getHp() > 0)
            {
                window.draw(pig.getSprite());
            }
        }

        /**
         * @brief Draw Bird object bird.
         * 
         * @param bird The Bird object to be drawn.
         */
        void renderBird(sf::RenderWindow& window, Bird& bird) {
            if (bird.getHp() > 0)
            {
                window.draw(bird.getSprite());
            }
        }

        /**
         * @brief Draw Obstacle object obstacle.
         * 
         * @param obstacle The Obstacle object to be drawn.
         */
        void renderObstacle(sf::RenderWindow& window, Obstacle& obstacle) {
            if (obstacle.getHp() > 0)
            {
                window.draw(obstacle.getSprite());
            }
        }

        void renderSlingShot(sf::RenderWindow& window, Slingshot& slingshot) {
            window.draw(slingshot.getSprite());
        }

        void renderStar(sf::RenderWindow& window, Star& star) {
            if (star.getHp() > 0)
            {
                window.draw(star.getSprite());
            }
        }
        
        void renderHeading(sf::RenderWindow& window, const std::string& string, float y) {
            sf::Text text;
            text.setFillColor(sf::Color::Magenta);
            text.setCharacterSize(70);
            text.setFont(latoBlack_);
            text.setString(string);
            text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, y);
            window.draw(text);
        }

        void renderHighScores(sf::RenderWindow& window, Scores highscores) {
            sf::Text text;
            text.setFillColor(sf::Color::Magenta);
            text.setCharacterSize(40);
            text.setFont(latoBold_);
            int width = window.getSize().x / 2;
            int pos_y = 250;
            text.setString("Top 5:");
            text.setPosition(width - text.getGlobalBounds().width / 2, pos_y);
            window.draw(text);
            text.setFont(latoRegular_);
            pos_y = pos_y + 50;
            for (auto pair : highscores) {
                if (pair.second != "") {
                    text.setString(pair.second + ": " + std::to_string(pair.first));
                    text.setPosition(width - text.getGlobalBounds().width / 2, pos_y);
                    pos_y = pos_y + 45;
                    window.draw(text);
                }
            }
        }

        void renderBackground(sf::RenderWindow& window, bool isLevelBackground) {
            float texture_x = static_cast<float>(levelBackgroundTexture_.getSize().x);
            float texture_y = static_cast<float>(levelBackgroundTexture_.getSize().y); 
            sf::Sprite sprite;
            if (isLevelBackground){ 
                sprite.setTexture(levelBackgroundTexture_);
            }
            else { 
                sprite.setTexture(menuBackgroundTexture_);
                texture_x = static_cast<float>(menuBackgroundTexture_.getSize().x);
                texture_y = static_cast<float>(menuBackgroundTexture_.getSize().y); 
            }
            
            // Calculate the scale factors
            float window_x = static_cast<float>(window.getSize().x);
            float window_y = static_cast<float>(window.getSize().y);

            float x_scale = 1.01 * window_x / texture_x;
            float y_scale = window_y / texture_y;

            // Set the sprite scale to stretch the background
            sprite.setScale(x_scale, y_scale);
            window.draw(sprite);
        }

        void renderInputBox(sf::RenderWindow& window, InputBox& box) {
            window.draw(box.getOutline());
            if (box.hasText()) {
                window.draw(box.getText());
            }
            else { window.draw(box.getPrompt()); }
        }

        void renderStars(sf::RenderWindow& window, int stars) {
            float size = 80;
            float space = 10;
            float x = window.getSize().x / 2 - (3 * size + 2 * space) / 2;
            float y = 150;
            // draw stars
            sf::Sprite sprite;
            sprite.setTexture(starTexture_);
            // scale sprite
            sprite.setScale(size / starTexture_.getSize().x, size / starTexture_.getSize().y);
            for (int i = 0; i < stars; ++i) {
                sprite.setPosition(x, y);
                window.draw(sprite);
                x += size + space;
            }
            // draw outlines

            sprite.setTexture(starOutlineTexture_);
            for (int i = 0; i < (3 - stars); ++i) {
                sprite.setPosition(x, y);
                window.draw(sprite);
                x += size + space;
            }
        }

        void renderLevelInfo(sf::RenderWindow& window, int pigs, int birds, int score) {
            // text object to draw texts
            sf::Text text;
            text.setFillColor(sf::Color::Magenta);
            text.setCharacterSize(30);
            text.setFont(latoRegular_);
            // pigs left text
            text.setString("Pigs left: " + std::to_string(pigs));
            float width = text.getGlobalBounds().width;
            float height = text.getGlobalBounds().height;
            float space = 10;
            float pos_x = window.getSize().x - width - 2 * space;
            float pos_y = space + height;
            text.setPosition(pos_x, pos_y);
            pos_y += space + height;
            window.draw(text);
            // birds left text
            text.setString("Birds left: " + std::to_string(birds));
            width = text.getGlobalBounds().width;
            height = text.getGlobalBounds().height;
            pos_x = window.getSize().x - width - 2 * space;
            text.setPosition(pos_x, pos_y);
            pos_y += space + height;
            window.draw(text);
            // score text
            // birds left text
            text.setString("Score: " + std::to_string(score));
            width = text.getGlobalBounds().width;
            height = text.getGlobalBounds().height;
            pos_x = window.getSize().x - width - 2 * space;
            text.setPosition(pos_x, pos_y);
            window.draw(text);
        }

    private:
        sf::Texture starTexture_;
        sf::Texture starOutlineTexture_;
        sf::Font latoRegular_;
        sf::Font latoBold_;
        sf::Font latoBlack_;
        sf::Texture levelBackgroundTexture_;
        sf::Texture menuBackgroundTexture_;
};

