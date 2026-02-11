#pragma once

#include "obstacle.hpp"

/**
 * @brief Represents a GlassObstacle in the game.
 * 
 * The GlassObstacle is a destructible obstacle with specific properties, including sound and texture,
 * and reacts with a distinct sound when interacted with.
 */
class GlassObstacle : public Obstacle {
public:
     /**
     * @brief Constructs the GlassObstacle object.
     * 
     * Initializes the GlassObstacle with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction and restitution.
     * The attributes described more specific in obstacle.hpp.
     */
    GlassObstacle (double x, double y) : Obstacle (8, x, y, 10, 40, "../src/soundfiles/glass.wav", "../src/imagefiles/glass.png", 1, 1, 0.1) {}

    /**
     * @brief Destructor for the GlassObstacle.
     */
    ~GlassObstacle () {}
};

/**
 * @brief Represents a StoneObstacle in the game.
 * 
 * The StoneObstacle is a sturdy, destructible obstacle with its own specific sound and texture,
 * and it reacts with a distinct sound when interacted with.
 */
class StoneObstacle : public Obstacle {
public:
     /**
     * @brief Constructs the StoneObstacle object.
     * 
     * Initializes the StoneObstacle with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction and restitution.
     * The attributes described more specific in obstacle.hpp.
     */
    StoneObstacle (double x, double y) : Obstacle (50, x, y, 30, 30, "../src/soundfiles/stone.wav", "../src/imagefiles/stone.png", 2, 1, 0.1) {}

    /**
     * @brief Destructor for the StoneObstacle.
     */
    ~StoneObstacle () {}
};

/**
 * @brief Represents a WoodObstacle in the game.
 * 
 * The WoodObstacle is a destructible obstacle with its own specific sound and texture,
 * and it reacts with a distinct sound when interacted with.
 */
class WoodObstacle : public Obstacle {
public:
     /**
     * @brief Constructs the WoodObstacle object.
     * 
     * Initializes the WoodObstacle with specific attributes: initialHp, x-coordinate, y-coordinate, width, height,
     * soundFilePath, textureFilePath, density, friction and restitution.
     * The attributes described more specific in obstacle.hpp.
     */
    WoodObstacle (double x, double y) : Obstacle (20, x, y, 60, 10, "../src/soundfiles/wood.wav", "../src/imagefiles/wood.png", 1.5, 1, 0.1) {}

    /**
     * @brief Destructor for the StoneObstacle.
     */
    ~WoodObstacle () {}
};