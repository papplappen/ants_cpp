struct Ant;
#pragma once

#include <algorithm>
#include <forward_list>

#include "Utils.hpp"
#include "Vec.hpp"
#include "Home.hpp"
#include "Food.hpp"

struct Ant
{
    Vec pos;
    Vec dir;

    Vec screenSize;

    std::forward_list<Home> &homes;
    std::forward_list<Food> &foods;

    int counter = 0;
    const double SPEED = 3;
    const int STEPS_BETWEEN_SIGNAL = int(75 / SPEED);

    const double VIEW_CONE_RADIUS = 100;

    const double RANDOM_ROTATE_MAX = radians(30);
    const double RANDOM_ROTATE_MIN = radians(15);

    double mood = 0;
    const double FEAR_MOOD = -0.25;
    const double FOOD_MOOD = 1;
    const double DELIVERY_MOOD = 1;
    const double MAX_MOOD = 5;

    double life;
    const double LIFE_DRAIN = 0.001;
    const double LIFE_PANIC = 0.1;

    bool carries_food;

    Ant(Vec pos_, Vec dir_, Vec screenSize_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_);

    void update();
    void show();

    void wallCollision(Vec screenSize);

    void deliverFood(Home &h);
    void pickupFood(Food &f);

    void changeMood(double m);

    bool inViewCone(const Vec &p_rel);

    static bool dead(const Ant &a);
};