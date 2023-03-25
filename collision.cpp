//
// Created by Centauria V. CHEN on 2023/3/25.
//

#include "collision.h"

double collide_prob(Point a0, double ta0, Point a1, double ta1,
                    Point b0, double tb0, Point b1, double tb1)
{
    if (ta1 < tb0 || tb1 < ta0) return 0;
    return 1;
}