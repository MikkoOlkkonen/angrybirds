#pragma once

#include "bird.hpp"
#include <memory>


class Slingshot {
public:
    /**
     * @brief Construct a new Slingshot object
     * 
     */
    Slingshot() :  bird_(nullptr), width_(50), height_(100), birdplace_(145, 505) { createSprite(); };
    ~Slingshot() = default;

    void createSprite() {
        if (!texture_.loadFromFile("../src/imagefiles/slingshot.png")) {
            // throw error
            throw std::runtime_error("Error loading texture file for slingshot!");
        }
        sprite_.setTexture(texture_);
        sf::FloatRect bounds = sprite_.getLocalBounds();
        sprite_.setOrigin(bounds.width / 2, bounds.height / 2);
        float texture_x = static_cast<float>(texture_.getSize().x);
        float texture_y = static_cast<float>(texture_.getSize().y);
        float x_scale = width_ / texture_x;
        float y_scale = height_ / texture_y;
        sprite_.setScale(x_scale, y_scale);

        sprite_.setPosition(birdplace_.x, birdplace_.y + height_/2);
    }
        
    sf::Sprite getSprite() { return sprite_; }

    bool hasBird () {
        if (!bird_) { return false; }
        else { return true; }
    }

    void setBird (std::shared_ptr<Bird>& bird) { bird_ = bird; }

    void shoot() {}

    int getX() { return birdplace_.x; }
    int getY() { return birdplace_.y + 15; }

private:
    std::shared_ptr<Bird> bird_;
    sf::Sprite sprite_;
    sf::Texture texture_;
    float width_;
    float height_;
    sf::Vector2i birdplace_;
};