#include "object.hpp"
#include <SFML/Audio.hpp>
#include <iostream>


Object::Object(int initialHp, double x, double y, double width, double height, const std::string& soundFilePath, const std::string& textureFilePath,
           double density, double friction, double restitution)
        : hp_(initialHp), destroyed_(false), width_(width), height_(height), position_(sf::Vector2f(x, y))
{
    // get texture from file
    if (!texture_.loadFromFile(textureFilePath)) {
        throw std::runtime_error("Error loading texture file: " + textureFilePath + "!");
    }

    // get sound from file
    if (!soundBuffer_.loadFromFile(soundFilePath)) {
        throw std::runtime_error("Error loading sound file: " + soundFilePath + "!");
    }
    sound_.setBuffer(soundBuffer_);
    sound_.setVolume(40);

    createSprite();
    setPhysicsProperties(density, friction, restitution);   // Set the object's physical properties
    bodyDef_.position.Set(x / 100, y / 100);                // Set position and other attributes, Convert to Box2D unit
}

void Object::takeDamage(int amount) {
    hp_ -= amount;
    if (hp_ <= 0) {
        hp_ = 0;     // Ensuring hp doesn't go below zero
        destroyed_ = true;
    }
}

void Object::speak() {
    sound_.play();
}

void Object::createSprite() {
    sprite_.setTexture(texture_);

    sf::FloatRect bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.width / 2, bounds.height / 2);

    float texture_x = static_cast<float>(texture_.getSize().x);
    float texture_y = static_cast<float>(texture_.getSize().y);
    float x_scale = width_ / texture_x;
    float y_scale = height_ / texture_y;

    sprite_.setScale(x_scale, y_scale);
    sprite_.setPosition(getX(), getY());
}

void Object::setShapeRectangle() {
    setOriginCenter();
    bodyDef_.position.Set(getX()/100, getY()/100);
    rectangleShape_.SetAsBox(getWidth()/200, getHeight()/200);
    fixtureDef_.shape = &rectangleShape_;
}

void Object::setShapeCircle() {
    setOriginCenter();
    bodyDef_.position.Set(getX()/100, getY()/100);
    circleShape_.m_radius = width_/200.0f;
    fixtureDef_.shape = &circleShape_;
}
