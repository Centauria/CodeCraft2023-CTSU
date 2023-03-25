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
double Trace::collide_prob(Trace trace) const
{
    if (t_end < trace.t_start || trace.t_end < t_start) return 0;
    double t0 = std::max(t_start, trace.t_start);
    double t1 = std::min(t_end, trace.t_end);
    Point a0_intersect = proportion(start, end, (t0 - t_start) / (t_end - t_start));
    Point a1_intersect = proportion(start, end, (t1 - t_start) / (t_end - t_start));
    Point b0_intersect = proportion(trace.start, trace.end, (t0 - trace.t_start) / (trace.t_end - trace.t_start));
    Point b1_intersect = proportion(trace.start, trace.end, (t1 - trace.t_start) / (trace.t_end - trace.t_start));
    auto distance =
            [a0_intersect,
             a1_intersect,
             b0_intersect,
             b1_intersect,
             t0, t1](double t) {
                auto lambda = (t - t0) / (t1 - t0);
                auto at = proportion(a0_intersect, a1_intersect, lambda);
                auto bt = proportion(b0_intersect, b1_intersect, lambda);
                return (at - bt).norm();
            };
    auto [_, min_distance] = minimize(distance, t0, t1);
    return HardSigmoid(pow(2, 1 - min_distance), 0, 1);
}
