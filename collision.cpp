//
// Created by Centauria V. CHEN on 2023/3/25.
//

#include "collision.h"
#include "function.h"

#include <algorithm>

double collide_prob(Point a0, double ta0, Point a1, double ta1,
                    Point b0, double tb0, Point b1, double tb1)
{
    if (ta1 < tb0 || tb1 < ta0) return 0;
    double t0 = std::max(ta0, tb0);
    double t1 = std::min(ta1, tb1);
    Point a0_intersect = proportion(a0, a1, (t0 - ta0) / (ta1 - ta0));
    Point a1_intersect = proportion(a0, a1, (t1 - ta0) / (ta1 - ta0));
    Point b0_intersect = proportion(b0, b1, (t0 - tb0) / (tb1 - tb0));
    Point b1_intersect = proportion(b0, b1, (t1 - tb0) / (tb1 - tb0));
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