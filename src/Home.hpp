struct Home;
#pragma once

#include <forward_list>

#include "Vec.hpp"
#include "Ant.hpp"
#include "Food.hpp"

struct Home
{
    Vec pos;
    double food;
    int counter;

    std::forward_list<Ant> &ants;
    std::forward_list<Home> &homes;
    std::forward_list<Food> &foods;

    const double RADIUS = 50;
    const int BIRTH_RATE = 10;
    const double NEW_ANT_COST = 3;
    const double NEW_ANT_MINIMUM = 20;

    Home(Vec pos_, std::forward_list<Ant> &ants_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_);

    void update();
    void show();

    void newAnt();

    bool takeFood(double f);
};