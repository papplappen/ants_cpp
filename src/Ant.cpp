#include "Ant.hpp"

Ant::Ant(Vec pos_, Vec dir_, Vec screenSize_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_) : homes(homes_), foods(foods_)
{
    pos = pos_;
    dir = dir_;
    screenSize = screenSize_;
}

void Ant::update()
{
    Vec tempDir = Vec(0, 0);
    if (!carries_food)
    {
        for (Food f : foods)
        {
            Vec relativeFoodPos = Vec::sub(f.pos, pos);
            if (relativeFoodPos.mag() < f.RADIUS)
            {
                pickupFood(f);
            }
            if (inViewCone(relativeFoodPos))
            {
                tempDir = relativeFoodPos;
            }
        }
    }
    for (Home h : homes)
    {
        Vec relativeHomePos = Vec::sub(h.pos, pos);
        if (relativeHomePos.mag() <= h.RADIUS)
        {
            if (carries_food)
            {
                deliverFood(h);
            }
            if (h.takeFood(1 - life))
            {
                life = 1;
            }
        }
        if (carries_food && inViewCone(relativeHomePos))
        {
            tempDir = relativeHomePos;
        }
    }
    if (tempDir.magSq() == 0)
    {
        //SIGNAL STUFF
    }
    dir.add(tempDir);
    dir.normalize();

    {
        double r = std::max(RANDOM_ROTATE_MIN, (1 - life) * RANDOM_ROTATE_MAX);
        dir.rotate(random(-r, r));
    }
    pos.add(Vec::mult(dir, SPEED));
    wallCollision(screenSize);

    mood *= 0.99;

    counter++;
    if (counter >= STEPS_BETWEEN_SIGNAL)
    {
        counter = 0;
        if (abs(mood) > 0.01)
        {
            //SIGNAL STUFF
        }
    }

    life -= LIFE_DRAIN;
    if (mood >= 0 && life < LIFE_PANIC)
    {
        mood = FEAR_MOOD;
    }
}

void Ant::show() {}

void Ant::wallCollision(Vec screenSize)
{
    if (pos.x > screenSize.x)
    {
        pos.x = screenSize.x;
        dir.x = -dir.x;
    }
    if (pos.x < 0)
    {
        pos.x = 0;
        dir.x = -dir.x;
    }
    if (pos.y > screenSize.y)
    {
        pos.y = screenSize.y;
        dir.y = -dir.y;
    }
    if (pos.y < 0)
    {
        pos.y = 0;
        dir.y = -dir.y;
    }
}

void Ant::deliverFood(Home &h)
{
    carries_food = false;
    changeMood(FOOD_MOOD);
    h.food += 1;
    dir = Vec::random2D();
}
void Ant::pickupFood(Food &f)
{
    f.take(2 - life);
    carries_food = true;
    changeMood(FOOD_MOOD);
    dir.mult(-1);
    life = 1;
}

void Ant::changeMood(double m)
{

    mood = std::min(MAX_MOOD, std::max(-MAX_MOOD, mood + m));
}

bool Ant::inViewCone(const Vec &p_rel)
{
    return (Vec::dot(p_rel, dir) >= 0 && p_rel.mag() <= VIEW_CONE_RADIUS);
}
bool Ant::dead(const Ant &a)
{
    return a.life <= 0;
}