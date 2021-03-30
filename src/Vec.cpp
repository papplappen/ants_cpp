#include "Vec.hpp"

Vec::Vec()
{
    x = 0;
    y = 0;
}
Vec::Vec(double x_, double y_)
{
    x = x_;
    y = y_;
}

void Vec::normalize()
{
    double magnitude = mag();
    x /= magnitude;
    y /= magnitude;
}
void Vec::add(Vec a)
{
    x += a.x;
    y += a.y;
}
void Vec::mult(double factor)
{
    x *= factor;
    y *= factor;
}
void Vec::rotate(double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    double xt = x * c - y * s;
    y = x * s + y * c;
    x = xt;
}

Vec Vec::normalized() const
{
    double magnitude = mag();

    return Vec(x / magnitude, y / magnitude);
}
double Vec::mag() const
{
    return sqrt(x * x + y * y);
}
double Vec::magSq() const
{
    return x * x + y * y;
}
Vec mult(const Vec &a, double factor)
{
    return Vec(a.x * factor, a.y * factor);
}
Vec Vec::sub(const Vec &a, const Vec &b)
{
    return Vec(a.x - b.x, a.y - b.y);
}
double Vec::dist(const Vec &a, const Vec &b)
{
    return Vec::sub(a, b).mag();
}
double Vec::dot(const Vec &a, const Vec &b)
{
    return a.x * b.x + a.y * b.y;
}
Vec Vec::random2D()
{
    double angle = (rand() / double(RAND_MAX)) * M_PI * 2;
    return Vec(sin(angle), cos(angle));
}

std::ostream &operator<<(std::ostream &os, const Vec &vec)
{
    return os << vec.x << " | " << vec.y;
}