#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/box2d.h>

/**
 * @brief Parent class for bird, pig, and obstacle classes.
 * 
 * This class serves as a base for all interactive objects in the game. It combines
 * physical properties, graphical representation, and sound effects.
 * 
 */

class Object {
    public:

        /**
         * @brief Constructor to initialize an Object in the game.
         * 
         * Creates an object with specific physical and graphical properties, positioning it 
         * in the game and associating it with texture and sound assets.
         * 
         * @param initialHp Initial health points of the Object.
         * @param x X-coordinate of the Object's initial position.
         * @param y Y-coordinate of the Object's initial position.
         * @param width Width of the Object in Box2D units (meters).
         * @param height Height of the Object in Box2D units (meters).
         * @param soundFilePath Path to the sound file associated with the Object for audio effects.
         * @param textureFilePath Path to the texture file used to render the Object's appearance.
         * @param density The mass density of the Object, influencing its mass and inertia.
         * @param friction The coefficient of friction, determining how the Object interacts with surfaces.
         * @param restitution The coefficient of restitution (bounciness), defining how the Object responds to collisions.
         */

        Object(int initialHp, double x, double y, double width, double height, const std::string& soundFilePath, const std::string& textureFilePath,
           double density, double friction, double restitution);

        /**
         * @brief Virtual destructor for the Object.
         */
        virtual ~Object() {};

        /**
         * @brief Apply damage to the Object.
         * 
         * Reduces the object's health points by the specified amount.
         * 
         * @param amount The amount of damage to apply.
         */
        void takeDamage(int amount);

       /**
         * @brief Check if the Object is destroyed.
         * 
         * @return Boolean value 'true' if the Object is destroyed, 'false' otherwise.
         */
        bool isDestroyed() { return destroyed_; }

        /**
         * @brief Destroy the Object.
         * 
         * Marks the object as destroyed, making it non-functional in the game.
         */
        void destroy() { destroyed_ = true; }

        /**
         * @brief Get the current health points of the Object.
         * 
         * @return int The health points of the Object.
         */
        int getHp() { return hp_; }

        /**
         * @brief Play the sound associated with the Object.
         */
        void speak();

        /**
         * @brief Get the number of times the Object has "spoken."
         * 
         * @return int The speak count.
         */
        int getSpeakCount() { return speakCount_; }

        /**
         * @brief Increment the speak count.
         */
        void incrementSpeak() { speakCount_++; }

        /**
         * @brief Get the position (coordinates) of the Object.
         * 
         * x and y can be accessed with .first and .second outside of the class
         * 
         * @return sf::Vector2f The position of the Object as a 2D vector.
         */
        sf::Vector2f GetPosition() { return position_; }

        /**
         * @brief Create the graphical sprite for the Object.
         */
        void createSprite();

        /**
         * @brief Get the sprite representing the Object.
         * 
         * @return sf::Sprite& A reference to the sprite.
         */
        sf::Sprite& getSprite() { return sprite_; }
        
        /**
         * @brief Update the graphics Object's position (pure virtual function).
         * 
         * Must be implemented by derived classes to handle specific update logic.
         */
        virtual void Update() = 0;

        /**
         * @brief Center the origin of the Object for transformations.
         */
        void setOriginCenter() {
            originX_ = width_ / 2.0f;
            originY_ = height_ / 2.0f;
        }

        /**
         * @brief Get the X-coordinate of the Object.
         * 
         * @return float The X-coordinate.
         */
        float getX() const { return position_.x; }

        /**
         * @brief Get the Y-coordinate of the Object.
         * 
         * @return float The Y-coordinate.
         */
        float getY() const { return position_.y; }

        /**
         * @brief Set the X-coordinate of the Object.
         * 
         * @param x The new X-coordinate.
         */
        void setX(double x) { position_.x = x; };

        /**
         * @brief Set the Y-coordinate of the Object.
         * 
         * @param y The new Y-coordinate.
         */
        void setY(double y) { position_.y = y; };

        /**
         * @brief Get the X-origin of the Object.
         * 
         * @return float The X-origin.
         */
        float getOriginX() const { return originX_; }

        /**
         * @brief Get the Y-origin of the Object.
         * 
         * @return float The Y-origin.
         */
        float getOriginY() const { return originY_; }

        /**
         * @brief Get the width of the Object.
         * 
         * @return float The width.
         */
        float getWidth() const { return width_; }

        /**
         * @brief Get the height of the Object.
         * 
         * @return float The height.
         */
        float getHeight() const { return height_; }

        /**
         * @brief Set the dimensions of the Object (for rectangle shapes).
         * 
         * @param w The width.
         * @param h The height.
         */
        void setDimensions(float w, float h) {
            width_ = w;
            height_ = h;
        }

        /**
         * @brief Set the Object as static (non-moving).
         */
        void setStatic() { bodyDef_.type = b2_staticBody; };

        /**
         * @brief Set the Object as dynamic (movable).
         */
        void setDynamic() { bodyDef_.type = b2_dynamicBody; };

        /**
         * @brief Change the Body type to static during runtime.
         */
        void setBodyStatic() { body_->SetType(b2_staticBody); };

        /**
         * @brief Change the Body type to dynamic during runtime.
         */
        void setBodyDynamic() { body_->SetType(b2_dynamicBody); };

        /**
         * @brief Enable fixed rotation for the Object.
         */
        void setFixedRotation() { body_->SetFixedRotation(true); };

        /**
         * @brief Disable fixed rotation for the Object.
         */
        void resetFixedRotation() { body_->SetFixedRotation(false); };

        /**
         * @brief Set the shape of the Object to rectangle (Box2D).
         */
        void setShapeRectangle();

        /**
         * @brief Set the shape of the Object to circle (Box2D).
         */
        void setShapeCircle();
        
        /**
         * @brief Set the physical properties for the Object in the physics simulation.
         * 
         * This method adjusts key parameters affecting the object's behavior in the
         * Box2D physics world, including its mass, surface interaction, and elasticity.
         * 
         * @param density The mass density of the object, influencing its inertia and weight.
         * @param friction The coefficient of friction, determining how the object interacts with surfaces.
         * @param restitution The coefficient of restitution (bounciness), defining how much energy is retained during collisions.
         */
        void setPhysicsProperties(double density, double friction, double restitution) {
            fixtureDef_.density = density;
            fixtureDef_.friction = friction;
            fixtureDef_.restitution = restitution;
        };

        /**
         * @brief Get the mass density of the Object.
         * 
         * @return double The mass density of the object.
         */
        double getDensity() const { return fixtureDef_.density; };

        /**
         * @brief Get the coefficient of friction of the Object.
         * 
         * @return double The coefficient of friction.
         */
        double getFriction() const { return fixtureDef_.friction; };

        /**
         * @brief Get the coefficient of restitution (bounciness) of the Object.
         * 
         * @return double The coefficient of restitution.
         */
        double getRestitution() const { return fixtureDef_.restitution; };

        /**
         * @brief Get the linear velocity of the Object in the physics world.
         * 
         * @return b2Vec2 The current velocity of the object as a 2D vector.
         */
        b2Vec2 getVelocity() const { return body_->GetLinearVelocity(); };

        /**
         * @brief Set the velocity of the Object.
         * 
         * @param vx X-component of velocity
         * @param vy Y-component of velocity
         */
        void setVelocity(double vx, double vy) { body_->SetLinearVelocity(b2Vec2(vx, vy)); };

        /**
         * @brief Set the linear velocity for the Object in the physics world.
         * 
         * This updates the object's velocity by modifying its Box2D body.
         * 
         * @param v Velocity vector as a b2Vec2
         */
        void setVelocity(b2Vec2 v) { body_->SetLinearVelocity(v); };

        /**
         * @brief Get the speed of the Object.
         * 
         * This calculates the magnitude of the object's velocity vector.
         * 
         * @return double The speed of the object as a scalar value.
         */
        double getSpeed() const { return getVelocity().Length(); };

        /**
         * @brief Initialize the physics world with the specific object's properties.
         * 
         * This method is a pure virtual function that must be implemented by derived classes.
         * It is used to set up the physics representation of the object in the Box2D world.
         * 
         * @param world Reference to the Box2D physics world where the object will be initialized.
         */
        virtual void initializePhysicsWorld(b2World &world) = 0;

        /**
         * @brief Get the Box2D body of the Object.
         * 
         * @return b2Body* Pointer to the Box2D body.
         */
        b2Body* getBody() { return body_; };

        /**
         * @brief Get the Box2D fixture definition of the Object.
         * 
         * @return b2FixtureDef The fixture definition.
         */
        b2FixtureDef getFixture() const { return fixtureDef_; };

        /**
         * @brief Apply a force to the center of the Object.
         * 
         * @param force Force vector to apply.
         */
        void applyForce(b2Vec2 force) { body_->ApplyForceToCenter(force, true); };

        /**
         * @brief Apply an impulse to the Object.
         * 
         * @param impulse Impulse vector to apply.
         */
        void applyImpulse(b2Vec2 impulse) { body_->ApplyLinearImpulse(impulse, body_->GetPosition(), true); };

        /**
         * @brief Check if the Object is flying.
         * 
         * @return Boolean value 'true' if flying, 'false' otherwise.
         */
        bool isFlying() { return flying_; }

        /**
         * @brief Set the Object to a flying state.
         */
        void fly() { flying_ = true; }

        /**
         * @brief Set the Object to a landed (not flying) state.
         */
        void land() { flying_ = false; }



    protected:
        int hp_;                           // initial health points
        bool destroyed_ = false;           // Used to track if object is destroyed
        double originX_, originY_;         // center of the object
        double width_, height_;            // width and height of the object
        bool flying_ = false;
        int speakCount_ = 0;                                     

        sf::SoundBuffer soundBuffer_;     // Sound buffer holding sound data
        sf::Sound sound_;                 // Sound object used to play the sound

        sf::Vector2f position_;           // The position where the object is at
        sf::Texture texture_;             // Texture on gui
        sf::Sprite sprite_;

        b2Body* body_;
        b2BodyDef bodyDef_;
        b2FixtureDef fixtureDef_;

        b2CircleShape circleShape_;
        b2PolygonShape rectangleShape_;


          
};
