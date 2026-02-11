#pragma once

#include "object.hpp"
#include <box2d/box2d.h>
#include <userdata.hpp>

/**
 * @brief Parent class for all bird objects in the game.
 * 
 * The Bird class is an abstract base class that inherits from Object. It defines common 
 * behaviors and properties for all bird objects, such as the ability to perform a special 
 * action, and tracks their state within the game.
 */
class Bird : public Object{
public:
    /**
     * @brief Constructor for the Bird class.
     * 
     * Initializes a bird object with specific physical, graphical, and gameplay-related properties.
     * 
     * @param initialHp Initial health points of the bird.
     * @param x X-coordinate of the bird's initial position.
     * @param y Y-coordinate of the bird's initial position.
     * @param width Width of the bird in Box2D units (meters).
     * @param height Height of the bird in Box2D units (meters).
     * @param soundFilePath Path to the sound file associated with the bird.
     * @param textureFilePath Path to the texture file used to render the bird's appearance.
     * @param density Mass density of the bird, affecting its weight and inertia.
     * @param friction Friction coefficient of the bird, influencing its interaction with surfaces.
     * @param restitution Restitution coefficient (bounciness) of the bird.
     * @param action_used Initial state of the special action (used or not).
     */
    Bird(int initialHp, double x, double y, double width, double height, const std::string& soundFilePath, const std::string& textureFilePath, 
           double density, double friction, double restitution, bool action_used)
           : Object(initialHp, x, y, width, height, soundFilePath, textureFilePath,
           density, friction, restitution), special_action_used_(action_used) {
            setStatic();
            setShapeCircle();
        };

    /**
     * @brief Virtual destructor for the Bird class.
     */
    virtual ~Bird() {};

    /**
     * @brief Perform the bird's special action.
     * 
     * This method is a pure virtual function and must be overridden by derived classes 
     * to define specific special actions for each type of bird.
     */
    virtual void SpecialAction() = 0;

    /**
     * @brief Check if the bird's special action has been used.
     * 
     * @return Boolean value 'true' if the special action has been used, 'false' otherwise.
     */
    bool isSpecialActionUsed() { return special_action_used_; }

    /**
     * @brief Check if the bird is marked as dead.
     * 
     * @return Boolean value 'true' if the bird is dead, 'false' otherwise.
     */
    bool isDead() { return isKilled_; }

     /**
     * @brief Mark the bird as dead.
     */
    void kill() { isKilled_ = true; }

    /**
     * @brief Check if the bird has been shot.
     * 
     * @return Boolean value 'true' if the bird has been shot, 'false' otherwise.
     */
    bool isShot() { return isShot_; }

    /**
     * @brief Mark the bird as shot.
     */
    void shoot() { isShot_ = true; }

    /**
     * @brief Update the bird's position and orientation.
     * 
     * Synchronizes the graphical representation of the bird with its physical position 
     * in the game.
     */
    void Update() {
        if (body_)
        {
            b2Vec2 position = body_->GetPosition();
            double angle = body_->GetAngle();

            setX(position.x * 100.0f);
            setY(position.y * 100.0f);

            // Convert the position from Box2D units to SFML pixels
            sprite_.setPosition(position.x * 100.0f, position.y * 100.0f);  // Convert meters to pixels
            sprite_.setRotation(angle * 180.0f / b2_pi);  // Convert radians to degrees
        }
    }

    /**
     * @brief Check if a given position is within the bounds of the bird's sprite.
     * 
     * @param position The pixel position to check.
     * @param window The render window for coordinate transformation.
     * @return Boolean value 'true' if the position is within the bird's bounds, 'false' otherwise.
     */
    bool inBounds(sf::Vector2i position, sf::RenderWindow& window) {
        sf::Vector2f globalPosition = window.mapPixelToCoords(position);
        sf::FloatRect bounds = getSprite().getGlobalBounds();
        bounds.contains(globalPosition);
        return bounds.contains(globalPosition);
        //return bounds.contains(static_cast<sf::Vector2f>(position));  // check if clicked position inside bounds 
    }

    /**
     * @brief Initialize the bird's physical representation in the game.
     * 
     * This method sets up the bird's body and fixtures in the Box2D physics world.
     * 
     * @param world Reference to the Box2D world where the bird will be added.
     */
    void initializePhysicsWorld(b2World &world) {
        Userdata* data = new Userdata;
        data->object = this;
        data->objecttype = "bird";
        b2BodyUserData userdata;
        userdata.pointer = reinterpret_cast<uintptr_t>(data);
        bodyDef_.userData = userdata;
        bodyDef_.fixedRotation = true;
        body_ = world.CreateBody(&bodyDef_);
        body_->CreateFixture(&fixtureDef_);
    }


protected:
    bool special_action_used_ = false;  // flag to see if special action has been used
    bool isKilled_ = false;             // flag to see if the bird is dead
    bool isShot_ = false;               // flag to see if the bird has been shot
};