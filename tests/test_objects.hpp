#pragma once

#include <iostream>
#include "bird_types.hpp"
#include "leveldata.hpp"

// test Object class if object is correctly destroyed
void testTakeDamage() {
    // create Bird to test
    YellowBird bird(100, 100);
    int original = bird.getHp();
    // take some damage
    bird.takeDamage(20);
    if (bird.getHp() == original - 20) {
        std::cout << "Test takeDamage succeeded!" << std::endl; 
    }
    else { std::cout << "Test takeDamage failed!" << std::endl; }
}

// test the "edge case" when bird collides with multiple objects
//void testMultipleCollision();