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
    Vector2D operator+(const Vector2D &v) const;
    Vector2D operator-(const Vector2D &v) const;
    double norm() const;
    double theta() const;

private:
    double x = 0.0;
    double y = 0.0;
};


#endif//CODECRAFTSDK_VECTOR2D_H
