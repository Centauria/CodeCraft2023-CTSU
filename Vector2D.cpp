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
Vector2D::Vector2D(double theta)
{
    this->x = cos(theta);
    this->y = sin(theta);
}
Vector2D Vector2D::operator+(const Vector2D &v) const
{
    return {x + v.x, y + v.y};
}
Vector2D Vector2D::operator-(const Vector2D &v) const
{
    return {x - v.x, y - v.y};
}
Vector2D Vector2D::operator*(double t) const
{
    return {t * x, t * y};
}
Vector2D operator*(double t, const Vector2D &v)
{
    return {t * v.x, t * v.y};
}
Vector2D Vector2D::operator/(double t) const
{
    return {x / t, y / t};
}
double Vector2D::norm() const
{
    return sqrt(x * x + y * y);
}
double Vector2D::theta() const
{
    return atan2(y, x);
}
double Vector2D::norm2() const
{
    return x * x + y * y;
}
double Vector2D::dot(const Vector2D &v) const
{
    return x * v.x + y * v.y;
}
Vector2D Vector2D::normalize() const
{
    double length = norm();
    if (length == 0) return {};
    return {x / length, y / length};
}
Vector2D &Vector2D::operator+=(const Vector2D &v)
{
    x += v.x;
    y += v.y;
    return *this;
}
Vector2D &Vector2D::operator*=(double t)
{
    x *= t;
    y *= t;
    return *this;
}
Vector2D &Vector2D::operator/=(double t)
{
    x /= t;
    y /= t;
    return *this;
}
