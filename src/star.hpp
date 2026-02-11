#pragma once

#include "pig.hpp"


class Star : public Pig {
public:
    /**
     * @brief Construct a new Star object, inherits pig
     * 
     * @param x 
     * @param y 
     */
    Star(double x, double y) : Pig(1, x, y, 40, 40, "../src/soundfiles/star.wav", "../src/imagefiles/star.png", 1, 1, 0.1) {
    }

    ~Star() {}

    void setData() {
        auto* userdata = reinterpret_cast<Userdata*>(getBody()->GetUserData().pointer);
        if (userdata) {
            userdata->objecttype = "star";
        }
    }
};