struct Ant;
#pragma once

#include <algorithm>
#include <forward_list>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "Utils.hpp"
//#include "Vec.hpp"
#include "Home.hpp"
#include "Food.hpp"

struct AntGPUData {
    glm::vec2 pos;
    glm::vec2 dir;
};

struct Ant {
    glm::vec2 pos;
    glm::vec2 dir;

    glm::vec2 screenSize;

    std::forward_list<Home> &homes;
    std::forward_list<Food> &foods;

    std::vector<AntGPUData> &gpuData;
    int gpuDataIndex;

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

    Ant(glm::vec2 pos_, glm::vec2 dir_, glm::vec2 screenSize_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_, std::vector<AntGPUData> &gpuData_);

    void update();
    void show();

    void wallCollision(glm::vec2 screenSize);

    void deliverFood(Home &h);
    void pickupFood(Food &f);

    void changeMood(double m);

    bool inViewCone(const glm::vec2 &p_rel);

    static bool dead(const Ant &a);
};