#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// Include necessary header files
#include "bird_types.hpp"
#include "pig_types.hpp"
#include "obstacle_types.hpp"
#include "ground.hpp"
#include "slingshot.hpp"
#include "star.hpp"


/**
 * @class LevelData
 * @brief Takes care of level function.
 *
 * The Level class is used to create and manage a level object,
 * where playing the game happens
 */

class LevelData {
public:
    /**
     * @brief Constructs a new Level object.
     * 
     * Initializes the Level object based on information read from textfile.
     * Creates filestream of textfile and reads each line from filestream. 
     * Creates stringstream of each line and processes the line token by token.
     * The first token informes if the line contains information about birds, pigs, 
     * or obstacles. 
     * Based on the rest of the tokens on the line, new Bird, Pig or Obstacle 
     * objects are created and added to corresponding vectors birds_, pigs_ and objects_.
     * @param filename The file where the level information is read.
     */
    LevelData() {}

    LevelData(int number) { loadFromFile(number); }

    /**
    * @brief Get vector of birds that belong to the level.
    * @returns Vector of Bird objects.
    */
    std::vector<std::shared_ptr<Bird>>& getBirds() { return birds_; }

    /**
    * @brief Get vector of pigs that belong to the level.
    * @returns Vector of Pig objects.
    */
    std::vector<std::shared_ptr<Pig>>& getPigs() { return pigs_; }

    /**
    * @brief Get vector of obstacles that belong to the level.
    * @returns Vector of Obstacle objects.
    */
    std::vector<std::shared_ptr<Obstacle>>& getObstacles() { return obstacles_; }

    /**
     * @brief Get the Ground object
     * 
     * @return std::unique_ptr<Obstacle> 
     */
    std::shared_ptr<Ground> getGround() { return ground_; }

    std::shared_ptr<Star> getStar() { return star_; }

private:
    void loadFromFile(int number);

    std::vector<std::shared_ptr<Bird>> birds_;
    std::vector<std::shared_ptr<Pig>> pigs_;
    std::vector<std::shared_ptr<Obstacle>> obstacles_;
    std::shared_ptr<Ground> ground_;
    std::shared_ptr<Star> star_;
};
