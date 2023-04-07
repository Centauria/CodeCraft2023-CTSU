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
    auto step = 2;
    auto forward_index = std::min(long(path.data.size() - 1), p_min_index + step);
    auto backward_index = std::max(0L, p_min_index - step);
    auto a = path(forward_index);
    auto c = path(backward_index);
    auto v_t = (a - c).normalize();
    auto curve_step = 1;
    // 确定计算曲率的点
    auto a_index = p_min_index - curve_step;
    auto c_index = p_min_index + curve_step;
    while (a_index < 0 || a_index == p_min_index || a_index == c_index) a_index++;
    while (c_index >= path.data.size() || c_index == p_min_index || c_index == a_index) c_index--;
    std::vector<long> indexes{a_index, p_min_index, c_index};
    std::sort(indexes.begin(), indexes.end());
    auto curve = curvature(path(indexes[0]), path(indexes[1]), path(indexes[2]));
    auto f = 3.0 / (1 + curve);
    v_t *= f;
    auto v_r = 3.0 * r;
    auto v = f * (v_t + v_r).normalize();
    auto theta = angle_diff(v.theta(), velocity.theta());
    return {f, 1.0 * theta};
}
