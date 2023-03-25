//
// Created by Centauria V. CHEN on 2023/3/10.
//
#include "point.h"
#include <cmath>


double angle_diff(double a, double b)
{
    /*
     * 计算从角度b转向角度a所需最小角度（弧度制）
     * 逆时针为正，顺时针为负
     */
    double diff = a - b;
    int n = static_cast<int>(diff / M_PI);
    return diff - n * M_PI * 2;
}

Point proportion(Point a, Point b, double lambda)
{
    return a + (b - a) * lambda;
}

Object::Object(Point pos, Velocity vel)
{
    position = pos;
    velocity = vel;
}
Object Object::operator-(const Object &o) const
{
    return {position - o.position, velocity - o.velocity};
}
Wall::Wall(Point pos, Velocity vel, Point position, WallOrientation orientation) : Object(pos, vel)
{
    this->position = position;
    this->orientation = orientation;
}
double Wall::distance(Point pos)
{
    return 0;
}
