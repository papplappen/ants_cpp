#pragma once

#include "Vec.hpp"

struct Food
{
    Vec pos;
    double capacity;
    double INIT_CAPACITY;
    const double RADIUS = 25;

    Food(Vec pos_, double capacity_);

    void take(double f);
    static bool empty(const Food &f);
    void show();
};