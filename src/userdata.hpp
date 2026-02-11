#pragma once

#include "object.hpp"
#include <string>

struct Userdata
{
    std::string objecttype;
    Object* object = nullptr;
};