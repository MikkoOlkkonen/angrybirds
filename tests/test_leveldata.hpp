#pragma once

#include <iostream>
#include "leveldata.hpp"

void testInvalidLevelFile() {
    try {
        LevelData leveldata(8);
    }
    catch (const std::runtime_error& e) {
        // find the constant part of error message
        if (std::string(e.what()).find("Failed opening the file") != std::string::npos) { 
            std::cout << "Test invalidLevelFile succeeded!" << std::endl; 
        }
        else { std::cout << "Test invalidLevelFile failed!" << std::endl; }
    }
}

void testValidLevelFile() {
    LevelData data(1);
    if (!data.getBirds().empty()) {
        std::cout << "Test validLevelFile birds succeeded!" << std::endl; 
    } else { std::cout << "Test validLevelFile birds failed!" << std::endl; }

    if (!data.getPigs().empty()) {
        std::cout << "Test validLevelFile pigs succeeded!" << std::endl; 
    } else { std::cout << "Test validLevelFile pigs failed!" << std::endl; }

    if (!data.getObstacles().empty()) {
        std::cout << "Test validLevelFile obstacles succeeded!" << std::endl; 
    } else { std::cout << "Test validLevelFile obstacles failed!" << std::endl; }

    if (data.getGround() != nullptr) {
        std::cout << "Test validLevelFile ground succeeded!" << std::endl; 
    } else { std::cout << "Test validLevelFile ground failed!" << std::endl; }

    if (data.getStar() != nullptr) {
        std::cout << "Test validLevelFile star succeeded!" << std::endl; 
    } else { std::cout << "Test validLevelFile star failed!" << std::endl; }
}
