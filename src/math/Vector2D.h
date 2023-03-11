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
    [[nodiscard]] double norm() const;
    [[nodiscard]] double theta() const;

    // x，y属性的setter，getter方法
    void set_x(double x);
    [[nodiscard]] double get_x() const;
    void set_y(double y);
    [[nodiscard]] double get_y() const;

private:
    double x = 0.0;
    double y = 0.0;
};


#endif//CODECRAFTSDK_VECTOR2D_H
