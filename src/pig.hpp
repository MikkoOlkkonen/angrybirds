#pragma once

#include "object.hpp"
#include <string>
#include <box2d/box2d.h>
#include "userdata.hpp"

// class for Pig, inherits Object
class Pig : public Object{
    public:
        /**
         * @brief Construct a new Pig object
         * 
         *
         */
        Pig(int initialHp, double x, double y, double width, double height, const std::string& soundFilePath, const std::string& textureFilePath, 
            double density, double friction, double restitution)
            : Object(initialHp, x, y, width, height, soundFilePath, textureFilePath,
            density, friction, restitution) {
                setShapeCircle();
                setDynamic();
            }

        // destructor
        virtual ~Pig() {};

        /**
         * @brief Update the position and rotation of pig
         * 
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
         * @brief Initialize the physics body for this pig
         * 
         * @param world the physics world
         */
        void initializePhysicsWorld(b2World &world) {
            Userdata* data = new Userdata;
            data->object = this;
            data->objecttype = "pig";
            b2BodyUserData userdata;
            userdata.pointer = reinterpret_cast<uintptr_t>(data);
            bodyDef_.userData = userdata;
            bodyDef_.type = b2_dynamicBody;
            bodyDef_.fixedRotation = false;
            body_ = world.CreateBody(&bodyDef_);
            body_->CreateFixture(&fixtureDef_);
        }

        bool inBounds(sf::Vector2i position, sf::RenderWindow& window) {
            sf::Vector2f globalPosition = window.mapPixelToCoords(position);
            sf::FloatRect bounds = getSprite().getGlobalBounds();
            bounds.contains(globalPosition);
            return bounds.contains(globalPosition);
        }
};
