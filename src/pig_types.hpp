#pragma once

#include "pig.hpp"

class NormalPig : public Pig {
public:
    NormalPig (double x, double y) : Pig (20, x, y, 30.0, 30.0, "../src/soundfiles/pig.wav", "../src/imagefiles/pig.png",  1, 1, 0.1) {}
    ~NormalPig () {}
};

class KingPig : public Pig {
public:
    KingPig (double x, double y) : Pig (45, x, y, 50.0, 50.0, "../src/soundfiles/pig.wav", "../src/imagefiles/kingpig.png", 1, 1, 0.1) {}
    ~KingPig () {}
};