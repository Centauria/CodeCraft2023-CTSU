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

enum WallOrientation
{
    X,
    Y
};

class Wall : public Object
{
public:
    Wall(Point pos, Velocity vel, Point position, WallOrientation orientation);
    double distance(Point pos);

private:
    WallOrientation orientation;
};

#endif//CODECRAFTSDK_POINT_H
