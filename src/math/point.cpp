//
// Created by Centauria V. CHEN on 2023/3/10.
//
#include "point.h"
#include <cmath>


Point::Point() = default;

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

Point Point::operator-(const Point &p) const
{
    return {x - p.x, y - p.y};
}

Polar Point::polar() const
{
    return {sqrt(x * x + y * y), atan2(y, x)};
}

double angle_diff(double a, double b)
{
    /*
     * 计算从角度b转向角度a所需最小角度（弧度制）
     * 逆时针为正，顺时针为负
     */
    double diff = a - b;
    int n = static_cast<int>(diff / M_PI);
    return diff - n * M_2_PI;
}

double angle_diff(Polar a, Polar b)
{
    return angle_diff(a.theta, b.theta);
}