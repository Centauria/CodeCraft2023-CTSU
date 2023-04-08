//
// Created by Centauria V. CHEN on 2023/4/7.
//

#include "concept.h"
#include <iostream>

double forward_correction(CMatrix &obs)
{
    auto r_agg = obs.sum(1);
    auto m = *std::max_element(r_agg.begin(), r_agg.end());
    if (m <= 18)
    {
        return m / 18.0;
    }
    return 1;
}
double rotate_correction(CMatrix &obs)
{
    auto r_agg = obs.sum(1);
    if (r_agg[0] <= 3 && r_agg[4] <= 3) return 0;
    if (r_agg[0] <= 3) return -1.0;
    else if (r_agg[4] <= 3)
        return 1.0;
    return 0;
}
Vector2D Robot::action(const Path &path, CMatrix &map)
{
    auto p_min = std::min_element(path.data.cbegin(), path.data.cend(), [this](auto p1, auto p2) {
        return (position - get_point(p1)).norm() < (position - get_point(p2)).norm();
    });
    auto b = get_point(*p_min);
    auto r = b - position;
    auto p_min_index = p_min - path.data.cbegin();
    auto obs = observe(map);
    int break_preparation = 3;
    auto f = 4.0 / (1 + abs(path.curvature(p_min_index, 1))) *
             (1 - ReLU(double(p_min_index) - double(path.data.size()) + 1 + break_preparation) / break_preparation) *
             forward_correction(obs);
    auto v_t = f * path.tangent(p_min_index, 2);
    auto v_r = 3.0 * r;
    auto v = f * (v_t + v_r).normalize();
    auto theta = angle_diff(v.theta(), orientation);
    return {f, 2.0 * theta + rotate_correction(obs)};
}

double Robot::radius() const
{
    return item_type ? 0.53 : 0.45;
}
CMatrix Robot::observe(CMatrix &map)
{
    auto observe_distance = 3.0;
    CMatrix blocks{5, 9, [this, map, observe_distance](size_t y, size_t x) mutable {
                       auto y_unit = Vector2D{orientation};
                       auto x_unit = Vector2D{orientation - M_PI_2};
                       auto start = position - radius() * x_unit + radius() * y_unit;
                       auto x_step = 2 * radius() / (5 - 1);
                       auto y_step = observe_distance / (9 - 1);
                       auto index = get_index(start + x_step * y * x_unit + y_step * x * y_unit);
                       return map(index.y, index.x);
                   }};
    for (int j = 0; j < 5; ++j)
    {
        char cache = 3;
        for (int i = 0; i < 9; ++i)
        {
            if (cache != 0 && blocks(j, i) == 0) cache = 0;
            else if (cache == 0)
                blocks(j, i) = 0;
        }
    }
    return blocks;
}
