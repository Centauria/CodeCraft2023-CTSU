//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_POINT_H
#define CODECRAFTSDK_POINT_H

#include "Vector2D.h"


typedef struct Polar {
    double r;
    double theta;
} Polar;

class Point : Vector2D
{
};

double angle_diff(double a, double b);
double angle_diff(Polar a, Polar b);

#endif//CODECRAFTSDK_POINT_H
