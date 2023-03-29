//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_POINT_H
#define CODECRAFTSDK_POINT_H

#include "Vector2D.h"

#define EPSILON 1e-7

typedef Vector2D Point;
typedef Vector2D Velocity;

double angle_diff(double a, double b);

class Object
{
public:
    Object(Point pos, Velocity vel);
    virtual ~Object() = default;
    Object operator-(const Object &o) const;

    Point position;
    Velocity velocity;
};

template<class T>
T proportion(T a, T b, double lambda)
{
    return a + (b - a) * lambda;
}

#endif//CODECRAFTSDK_POINT_H
