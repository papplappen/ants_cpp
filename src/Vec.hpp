#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

struct Vec
{
    double x, y;
    Vec();
    Vec(double x_, double y_);

    void normalize();
    void add(Vec a);
    void mult(double factor);
    void rotate(double angle);

    double mag() const;
    double magSq() const;
    Vec normalized() const;

    static Vec mult(const Vec &a, double factor);
    static Vec sub(const Vec &a, const Vec &b);
    static double dist(const Vec &a, const Vec &b);
    static double dot(const Vec &a, const Vec &b);

    static Vec random2D();
};
std::ostream &operator<<(std::ostream &os, const Vec &vec);