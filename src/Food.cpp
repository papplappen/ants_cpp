#include "Food.hpp"

Food::Food(Vec pos_, double capacity_)
{
    pos = pos_;
    capacity = capacity_;
    INIT_CAPACITY = capacity_;
}

void Food::take(double f)
{

    capacity -= f;
}

bool Food::empty(const Food &f)
{
    return f.capacity <= 0;
}

void Food::show()
{
}