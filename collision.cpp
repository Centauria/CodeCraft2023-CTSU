//
// Created by Centauria V. CHEN on 2023/3/25.
//

#include "collision.h"
#include "function.h"

#include <algorithm>

Trace::Trace(Point start, double t_start, Point end, double t_end)
{
    this->start = start;
    this->t_start = t_start;
    this->end = end;
    this->t_end = t_end;
}
double Trace::operator*(const Trace &trace) const
{
    if (t_end < trace.t_start || trace.t_end < t_start) return 0;
    double t0 = std::max(t_start, trace.t_start);
    double t1 = std::min(t_end, trace.t_end);
    auto distance =
            [this, trace](double t) {
                auto at = *this * t;
                auto bt = trace * t;
                return (at - bt).norm2();
            };
    auto [_, min_distance] = minimize_quad(distance, t0, t1);
    return HardSigmoid(pow(2, 1 - min_distance), 0, 1);
}
Point Trace::operator*(double t) const
{
    return proportion(start, end, (t - t_start) / (t_end - t_start));
}
