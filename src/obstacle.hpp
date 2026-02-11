#pragma once

#include "object.hpp"
#include <string>
#include <box2d/box2d.h>
#include <userdata.hpp>
#include <iostream>

/**
 * @class Obstacle
 * @brief Represents a dynamic rectangular object in the game, inheriting from Object.
 * 
 * The Obstacle class initializes and manages the physics and graphical representation of dynamic obstacles
 * in the game. Obstacles are rectangular by default and use Box2D for physics simulation.
 */
class Obstacle : public Object {
    public:
        /**
         * @brief Constructs an Obstacle object with specified parameters.
         * 
         * @param initialHp Initial health points of the obstacle.
         * @param x X-coordinate of the obstacle's initial position.
         * @param y Y-coordinate of the obstacle's initial position.
         * @param width Width of the obstacle in Box2D units (meters).
         * @param height Height of the obstacle in Box2D units (meters).
         * @param soundFilePath Path to the sound file associated with the obstacle.
         * @param textureFilePath Path to the texture file associated with the obstacle.
         * @param density Density of the obstacle in the physics simulation.
         * @param friction Friction coefficient of the obstacle.
         * @param restitution Restitution (bounciness) of the obstacle.
         */
        Obstacle(int initialHp, double x, double y, double width, double height, const std::string& soundFilePath, const std::string& textureFilePath, 
        double density, double friction, double restitution)
            : Object(initialHp, x, y, width, height, soundFilePath, textureFilePath, density, friction, restitution) 
        {
            setShapeRectangle();
            setDynamic();
        }

        /**
         * @brief Destructor for the Obstacle class.
         */
        virtual ~Obstacle() {};

        /**
         * @brief Updates the obstacle's position and rotation based on physics simulation.
         * 
         * Synchronizes the graphical representation of the obstacle with its physics body.
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
         * @brief Initializes the physics world for the obstacle.
         * 
         * Creates a Box2D body and fixture for the obstacle and assigns user data.
         * 
         * @param world The Box2D world in which the obstacle will exist.
         * @throws std::runtime_error if the fixture shape is not set before creation.
         */
        void initializePhysicsWorld(b2World &world) {
            auto* data = new Userdata;
            data->object = this;
            data->objecttype = "obstacle";
            body_ = world.CreateBody(&bodyDef_);
            body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
            if (!fixtureDef_.shape) {
                throw std::runtime_error("Fixture shape not set before creating the fixture");
            }
            body_->CreateFixture(&fixtureDef_);
        }

        /**
         * @brief Checks if a given position is within the bounds of the obstacle's sprite.
         * 
         * @param position The mouse position in window coordinates.
         * @param window The SFML window used to map pixel coordinates to world coordinates.
         * @return Boolean value 'true' if the position is within the obstacle's bounds, 'false' otherwise.
         */
        bool inBounds(sf::Vector2i position, sf::RenderWindow& window) {
            sf::Vector2f globalPosition = window.mapPixelToCoords(position);
            sf::FloatRect bounds = getSprite().getGlobalBounds();
            bounds.contains(globalPosition);
            return bounds.contains(globalPosition);
        }
};
