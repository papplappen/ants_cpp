#pragma once

//#include "Vec.hpp"
#include <glm/ext.hpp>
struct Food {
    glm::vec2 pos;
    double capacity;
    double INIT_CAPACITY;
    const double RADIUS = 25;

    Food(glm::vec2 pos_, double capacity_);

    void take(double f);
    static bool empty(const Food &f);
    void show();
};