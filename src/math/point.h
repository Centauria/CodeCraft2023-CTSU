//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_POINT_H
#define CODECRAFTSDK_POINT_H

typedef struct Polar {
    double r;
    double theta;
} Polar;

class Point
{
public:
    Point();
    Point(double x, double y);
    Point operator-(const Point &p) const;
    [[nodiscard]] Polar polar() const;

private:
    double x = 0.0;
    double y = 0.0;
};

double angle_diff(double a, double b);
double angle_diff(Polar a, Polar b);

#endif//CODECRAFTSDK_POINT_H
