#include "Ant.hpp"

Ant::Ant(glm::vec2 pos_, glm::vec2 dir_, glm::vec2 screenSize_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_, std::vector<AntGPUData> &gpuData_) : homes(homes_), foods(foods_), gpuData(gpuData_) {
    pos = pos_;
    dir = dir_;
    screenSize = screenSize_;
    gpuData.push_back(AntGPUData{pos, dir});
    gpuDataIndex = gpuData.size() - 1;
}

void Ant::update() {
    glm::vec2 tempDir = glm::vec2(0, 0);
    if (!carries_food) {
        for (Food f : foods) {
            glm::vec2 relativeFoodPos = f.pos - pos;
            if (glm::length(relativeFoodPos) < f.RADIUS) {
                pickupFood(f);
            }
            if (inViewCone(relativeFoodPos)) {
                tempDir = relativeFoodPos;
            }
        }
    }
    for (Home h : homes) {
        glm::vec2 relativeHomePos = h.pos - pos;
        if (glm::length(relativeHomePos) <= h.RADIUS) {
            if (carries_food) {
                deliverFood(h);
            }
            if (h.takeFood(1 - life)) {
                life = 1;
            }
        }
        if (carries_food && inViewCone(relativeHomePos)) {
            tempDir = relativeHomePos;
        }
    }
    if (glm::length2(tempDir) == 0) {
        //SIGNAL STUFF
    }
    dir += tempDir;
    dir = glm::normalize(dir);

    {
        double r = std::max(RANDOM_ROTATE_MIN, (1 - life) * RANDOM_ROTATE_MAX);
        glm::rotate(dir, float(random(-r, r)));
    }
    pos += dir * SPEED;
    wallCollision(screenSize);

    mood *= 0.99;

    counter++;
    if (counter >= STEPS_BETWEEN_SIGNAL) {
        counter = 0;
        if (abs(mood) > 0.01) {
            //SIGNAL STUFF
        }
    }

    life -= LIFE_DRAIN;
    if (mood >= 0 && life < LIFE_PANIC) {
        mood = FEAR_MOOD;
    }

    gpuData[gpuDataIndex] = AntGPUData{pos, dir};
}

void Ant::show() {}

void Ant::wallCollision(glm::vec2 screenSize) {
    if (pos.x > screenSize.x) {
        pos.x = screenSize.x;
        dir.x = -dir.x;
    }
    if (pos.x < 0) {
        pos.x = 0;
        dir.x = -dir.x;
    }
    if (pos.y > screenSize.y) {
        pos.y = screenSize.y;
        dir.y = -dir.y;
    }
    if (pos.y < 0) {
        pos.y = 0;
        dir.y = -dir.y;
    }
}

void Ant::deliverFood(Home &h) {
    carries_food = false;
    changeMood(FOOD_MOOD);
    h.food += 1;
    dir = glm::circularRand(1);
}
void Ant::pickupFood(Food &f) {
    f.take(2 - life);
    carries_food = true;
    changeMood(FOOD_MOOD);
    dir *= -1;
    life = 1;
}

void Ant::changeMood(double m) {
    mood = std::min(MAX_MOOD, std::max(-MAX_MOOD, mood + m));
}

bool Ant::inViewCone(const glm::vec2 &p_rel) {
    return (glm::dot(p_rel, dir) >= 0 && glm::length(p_rel) <= VIEW_CONE_RADIUS);
}
bool Ant::dead(const Ant &a) {
    return a.life <= 0;
}