#pragma once

#include "bird.hpp"
#include <box2d/box2d.h>

/**
 * @brief Represents a YellowBird in the game.
 * 
 * YellowBird has a special action that doubles its horizontal velocity when triggered.
 */
class YellowBird : public Bird {
public:
    /**
     * @brief Constructs a YellowBird object.
     * 
     * Initializes the YellowBird with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction, restitution and action_used.
     * The attributes described more specific in bird.hpp.
     */
    YellowBird (double x, double y) : Bird (300, x, y, 20, 20, "../src/soundfiles/yellowbird.wav", "../src/imagefiles/yellowbird.png", 1, 1, 0.1, false) {}
    
    /**
     * @brief Destructor for the YellowBird.
     */
    ~YellowBird () {}

    /**
     * @brief Executes the special action for the YellowBird.
     * 
     * This action doubles the bird's horizontal velocity, allowing it to gain a speed boost.
     * Once this action is used, it cannot be triggered again.
     */
    void SpecialAction() {
        special_action_used_ = true;
        b2Vec2 currentV = getVelocity();
        setVelocity(2 * currentV.x, currentV.y);
        // std::cout << currentV.x << std::endl;
        b2Vec2 newV = getVelocity();
        // std::cout << newV.x << std::endl;

    }
};

/**
 * @brief Represents a RedBird in the game.
 * 
 * RedBird does not have any special actions.
 */
class RedBird : public Bird {
public:
    /**
     * @brief Constructs a RedBird object.
     * 
     * Initializes the RedBird with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction, restitution and action_used.
     * The attributes described more specific in bird.hpp.
     */
    RedBird (double x, double y) : Bird (150, x, y, 30, 30, "../src/soundfiles/redbird.wav", "../src/imagefiles/redbird.png", 1, 1, 0.1, true) {}
    
    /**
     * @brief Destructor for the RedBird.
     */
    ~RedBird () {}

    /**
     * @brief Executes the special action for the RedBird.
     * 
     * The RedBird does not have any special action, so this function is a no-op.
     */
    void SpecialAction() {};
};