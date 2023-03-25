//
// Created by Centauria V. CHEN on 2023/3/11.
//
#include <cmath>

#include "Vector3D.h"


Vector3D::Vector3D() = default;
Vector3D::Vector3D(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector3D Vector3D::operator+(const Vector3D &v) const
{
    return {x + v.x, y + v.y, z + v.z};
}
Vector3D Vector3D::operator-(const Vector3D &v) const
{
    return {x - v.x, y - v.y, z + v.z};
}
Vector3D Vector3D::operator*(double t) const
{
    return {t * x, t * y, t * z};
}
Vector3D operator*(double t, const Vector3D &v)
{
    return {t * v.x, t * v.y, t * v.z};
}
Vector3D Vector3D::operator/(double t) const
{
    return {x / t, y / t, z / t};
}
double Vector3D::norm() const
{
    return sqrt(x * x + y * y + z * z);
}
double Vector3D::dot(const Vector3D &v) const
{
    return x * v.x + y * v.y + z * v.z;
}
Vector3D Vector3D::normalize() const
{
    double length = norm();
    if (length == 0) return {};
    return {x / length, y / length, z / length};
}
Vector3D &Vector3D::operator+=(const Vector3D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Vector3D &Vector3D::operator*=(double t)
{
    x *= t;
    y *= t;
    z *= t;
    return *this;
}
Vector3D &Vector3D::operator/=(double t)
{
    x /= t;
    y /= t;
    z /= t;
    return *this;
}
