//
// Created by Centauria V. CHEN on 2023/3/25.
//

#ifndef CODECRAFTSDK_COLLISION_H
#define CODECRAFTSDK_COLLISION_H

#include "point.h"

class Trace
{
public:
    Trace(Point start, double t_start, Point end, double t_end);

    // Collision probability can be directly calculated by
    // `traceA * traceB`
    double operator*(const Trace &trace) const;

    // Position when time is `t` can be calculated by
    // `trace * t`
    Point operator*(double t) const;

private:
    Point start;
    Point end;
    double t_start;
    double t_end;
};

#endif//CODECRAFTSDK_COLLISION_H
