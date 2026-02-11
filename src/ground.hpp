#pragma once

#include "obstacle.hpp"

/**
 * @class Ground
 * @brief Represents the ground object in the game, inheriting from Obstacle.
 * 
 * The Ground class defines a static, rectangular ground object that is unaffected by gravity.
 * It initializes specific properties like dimensions, image, and sound files.
 */
class Ground : public Obstacle {
public:
    /**
     * @brief Constructs the Ground object.
     * 
     * Initializes the Ground with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction and restitution.
     * The attributes described more specific in obstacle.hpp.
     */
    Ground () : Obstacle (9999, 680, 700, 1385, 210, "../src/soundfiles/ground.wav", "../src/imagefiles/ground.png", 0, 1, 0) {
        setStatic();
        setShapeRectangle();
    }

    /**
     * @brief Destructor for the Ground class.
     */
    ~Ground () {}

    /**
     * @brief Sets user data specific to the ground object.
     * 
     * Updates the Box2D user data pointer with information identifying the object type as "ground".
     */
    void setData() {
        auto* userdata = reinterpret_cast<Userdata*>(getBody()->GetUserData().pointer);
        if (userdata) {
            userdata->objecttype = "ground";
        }
    }

    /**
     * @brief Moves the ground object downward by a specified amount.
     * 
     * @param amount The vertical distance to move the ground in Box2D world units.
     */
    void moveBodyDown(double amount) {
        b2Vec2 currentPos = body_->GetPosition();
        b2Vec2 newPos = b2Vec2(currentPos.x, currentPos.y + amount);
        body_->SetTransform(newPos, 0);
    }
};