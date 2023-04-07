//
// Created by Centauria V. CHEN on 2023/4/7.
//

#include "concept.h"
Vector2D Robot::action(const Path &path)
{
    auto p_min = std::min_element(path.data.cbegin(), path.data.cend(), [this](auto p1, auto p2) {
        return (position - get_point(p1)).norm() < (position - get_point(p2)).norm();
    });
    auto b = get_point(*p_min);
    auto r = b - position;
    auto p_min_index = p_min - path.data.cbegin();
    auto f = 3.0 / (1 + path.curvature(p_min_index, 1));
    auto v_t = f * path.tangent(p_min_index, 2);
    auto v_r = 3.0 * r;
    auto v = f * (v_t + v_r).normalize();
    auto theta = angle_diff(v.theta(), velocity.theta());
    return {f, 2.0 * theta};
}
