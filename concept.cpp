//
// Created by Centauria V. CHEN on 2023/4/7.
//

#include "concept.h"
#include <iostream>

double Robot::forward_correction(const std::vector<double> &obs)
{
    auto m = std::min_element(obs.cbegin(), obs.cend());
    auto M = std::max_element(obs.cbegin(), obs.cend());
    if (*m >= 0.5 && *M <= 2)
    {
        return *M / 2.0;
    }
    return 1;
}
double Robot::rotate_correction(const std::vector<double> &obs)
{
    auto bar_width = 2 * radius() / double(obs.size() - 1);
    double integration = 0;
    for (int i = 0; i < obs.size(); ++i)
    {
        integration += bar_width * ReLU(3 - obs[i]) * (bar_width * i - radius());
    }
    return 8.0 * integration;
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
    auto f = 6.0 / (1 + abs(path.curvature(p_min_index, 1))) *
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
std::vector<double> Robot::observe(CMatrix &map)
{
    auto observe_distance = 3.0;
    auto x_step = 2 * radius() / (5 - 1);
    auto y_step = observe_distance / (9 - 1);
    auto y_unit = Vector2D{orientation};
    auto x_unit = Vector2D{orientation - M_PI_2};
    CMatrix blocks{5, 9, [this, map, x_step, y_step, x_unit, y_unit](size_t y, size_t x) mutable {
                       auto start = position - radius() * x_unit + radius() * y_unit;
                       auto index = get_index(start + x_step * y * x_unit + y_step * x * y_unit);
                       return map(index.y, index.x) ? 1 : 0;
                   }};
    for (int j = 0; j < 5; ++j)
    {
        char cache = 1;
        for (int i = 0; i < 9; ++i)
        {
            if (cache != 0 && blocks(j, i) == 0) cache = 0;
            else if (cache == 0)
                blocks(j, i) = 0;
        }
    }
    auto r_agg = blocks.sum(1);
    std::vector<double> result;
    result.reserve(r_agg.size());
    std::transform(r_agg.cbegin(), r_agg.cend(), std::back_inserter(result), [y_step](auto c) {
        return y_step * c;
    });
    return result;
}
