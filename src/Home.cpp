#include "Home.hpp"

Home::Home(Vec pos_, std::forward_list<Ant> &ants_, std::forward_list<Home> &homes_, std::forward_list<Food> &foods_) : ants(ants_), homes(homes_), foods(foods_)
{
    pos = pos_;
}

void Home::update()
{
    counter++;
    if (counter >= BIRTH_RATE && food >= NEW_ANT_MINIMUM)
    {
        counter = 0;
        food -= NEW_ANT_COST;
        newAnt();
    }
}

void Home::show()
{
}

void Home::newAnt()
{
    //ants.emplace_front(Ant(pos, Vec::random2D(), homes, foods));
}

bool Home::takeFood(double f)
{
    if (food == 0)
    {
        return false;
    }
    food -= f;
    if (food <= 0)
    {
        food = 0;
    }
    return true;
}