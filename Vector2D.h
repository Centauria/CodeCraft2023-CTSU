//
// Created by Centauria V. CHEN on 2023/3/11.
//

#ifndef CODECRAFTSDK_VECTOR2D_H
#define CODECRAFTSDK_VECTOR2D_H


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
    [[nodiscard]] double dot(const Vector2D &v) const;
    [[nodiscard]] Vector2D normalize() const;

    double x = 0.0;
    double y = 0.0;
};

Vector2D operator*(double t, const Vector2D &v);


#endif//CODECRAFTSDK_VECTOR2D_H
