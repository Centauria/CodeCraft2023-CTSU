//
// Created by Centauria V. CHEN on 2023/3/11.
//

#ifndef CODECRAFTSDK_VECTOR2D_H
#define CODECRAFTSDK_VECTOR2D_H

#include <string>


class Vector2D
{
public:
    Vector2D();
    Vector2D(double x, double y);
    explicit Vector2D(double theta);
    Vector2D operator+(const Vector2D &v) const;
    Vector2D operator-(const Vector2D &v) const;
    Vector2D operator*(double t) const;
    Vector2D operator/(double t) const;
    Vector2D &operator+=(const Vector2D &v);
    Vector2D &operator*=(double t);
    Vector2D &operator/=(double t);

    [[nodiscard]] double norm() const;
    [[nodiscard]] double theta() const;
    [[nodiscard]] double norm2() const;
    [[nodiscard]] double dot(const Vector2D &v) const;
    [[nodiscard]] Vector2D normalize() const;

    explicit operator std::string() const;

    double x = 0.0;
    double y = 0.0;
};

Vector2D operator*(double t, const Vector2D &v);
inline bool operator==(const Vector2D &lhs, const Vector2D &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

typedef Vector2D Point;
typedef Vector2D Velocity;

class CVector2D
{
public:
    CVector2D() : y(0), x(0){};
    CVector2D(int y, int x) : y(y), x(x){};
    CVector2D operator+(const CVector2D &v) const;
    CVector2D operator-(const CVector2D &v) const;

    explicit operator std::string() const;

    int x = 0.0;
    int y = 0.0;
};

typedef CVector2D Index;

#endif//CODECRAFTSDK_VECTOR2D_H
