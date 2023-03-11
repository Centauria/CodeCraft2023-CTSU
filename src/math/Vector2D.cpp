//
// Created by Centauria V. CHEN on 2023/3/11.
//
#include <cmath>

#include "Vector2D.h"


Vector2D::Vector2D() = default;
Vector2D::Vector2D(double x, double y)
{
    this->x = x;
    this->y = y;
}
Vector2D Vector2D::operator+(const Vector2D &v) const
{
    return {x + v.x, y + v.y};
}
Vector2D Vector2D::operator-(const Vector2D &v) const
{
    return {x - v.x, y - v.y};
}
double Vector2D::norm() const
{
    return sqrt(x * x + y * y);
}
double Vector2D::theta() const
{
    return atan2(y, x);
}