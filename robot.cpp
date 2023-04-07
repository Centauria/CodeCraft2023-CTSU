//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "argspecs.h"
#include "function.h"
#include "logging.h"
#include "robot.h"
#include "string_util.h"
#include <cmath>

Robot::Robot(int16_t id, double x, double y) : Object(Vector2D{x, y}, Vector2D{})
{
    this->id = id;
    this->orientation = 0.0;
    item_type = 0;
}

double Robot::ETA()
{
    // Estimated time of arrival
    std::vector<Point> ps;
    ps.reserve(targets.size());
    std::transform(targets.cbegin(), targets.cend(), std::back_inserter(ps), [](auto p) {
        return get_point(p);
    });
    return ETA(ps);
}
double Robot::ETA(const std::vector<Point> &points)
{
    if (points.empty()) return 0;
    const double a = 250.0 / 13;
    Vector2D r = position;
    auto v = velocity.norm();
    auto o = orientation;
    double time = 0;
    for (auto p: points)
    {
        r = p - r;
        auto d = r.norm();
        double r_o = angle_diff(r.theta(), o);
        double sc = abs(sinc(r_o));
        double arc_length;
        if (sc > 0.5)
        {
            arc_length = d / sc;
        } else
        {
            auto radius = d > 5 ? 5 : d;
            arc_length = d - radius + radius * M_PI;
        }
        double t = (5 - v) / a;
        double l = (v + a * t / 2) * t;
        time += (t + (arc_length - l) / 5);
        o = r.theta();
        r = p;
        v = 2.5;
    }
    return time;
}

std::tuple<Trade, Point> Robot::step(double delta)
{
//    if (id == 0)
//    {
//        std::cerr << "current:" << std::string(get_index(position));
//        if (targets.empty())
//            std::cerr << ", target empty ";
//        else
//            std::cerr << ", target:" << std::string(targets.front()) << " ";
//        auto [a, b] = see();
//        std::cerr << a << ", " << b << std::endl;
//    }
    auto action = calculate_dynamic(delta);
    forward(action.forward);
    rotate(action.rotate);

    if (!targets.empty())
    {
        auto p = targets.front();
        Trade t = calculate_trade();
        if ((position - get_point(targets.front())).norm() < 0.4 || get_index(position) == p)
        {
            targets.pop_front();
        }
        //        if (analog_targets.empty())
        //        {
        //            analyze_track();
        //        } else if (get_index(position) == get_index(analog_targets.front()))
        //        {
        //            analog_targets.pop_front();
        //        }
        return std::make_tuple(t, get_point(p));
    }
    return std::make_tuple(NONE, Point{});
}
void Robot::forward(double v) const
{
    std::cout << "forward " << +id << " " << v << std::endl;
}
void Robot::rotate(double w) const
{
    std::cout << "rotate " << +id << " " << w << std::endl;
}
void Robot::buy() const
{
    std::cout << "buy " << +id << std::endl;
}
void Robot::sell() const
{
    std::cout << "sell " << +id << std::endl;
}
void Robot::destroy() const
{
    std::cout << "destroy " << +id << std::endl;
}
void Robot::set_obstacle(const std::vector<std::unique_ptr<Object>> &obstacles)
{
    this->obstacles.clear();
    for (const auto &obs: obstacles)
    {
        this->obstacles.push_back(*obs);
    }
}
Action Robot::calculate_dynamic(double delta)
{
    if (targets.empty())
    {
        return {};
    }
    Vector2D r = get_point(targets.front()) - position;
    Action action_collision{};
    double weight_collision = 0;

    auto azimuth = r.theta();
    auto alpha = angle_diff(azimuth, orientation);
    auto p_error = LeakyReLU(r.norm());

    double f = 6 * p_error;
    f = HardSigmoid(f, -2.0, 6.0);
    auto [advice_wheel, max_val] = see();
    double advice_weight = 0.0;
    if (max_val <= 6) advice_weight = 0.0;
    else if (max_val <= 18)
        advice_weight = 0.0;
    if (alpha > M_PI / 4) advice_weight = 0.0;
    double alpha_weight = 1.0;
    double w = ((5 * alpha) * alpha_weight + 2 * advice_wheel * advice_weight) / (advice_weight + 1.0);
    w = HardSigmoid(w, -M_PI, M_PI);
    f -= (1.2 * abs(w));

    LOG("logs/ETA_error.log", string_format("%lf,%lf", ETA(), delta))
    Action action_target{f, w};
    double weight_target = 1;

    Vector2D AC{action_collision.forward, action_collision.rotate};
    Vector2D AT{action_target.forward, action_target.rotate};
    Vector2D A = (AC * weight_collision + AT * weight_target) / (weight_collision + weight_target);
    return {A.x, A.y};
}

Trade Robot::calculate_trade()
{
    if (workbench_id != -1 && (get_point(targets.front()) - position).norm() < 0.4)
    {
        if (isLoaded())
        {
            sell();
            return SELL;
        } else
        {
            buy();
            return BUY;
        }
    }
    return NONE;
}
bool Robot::isLoaded() const
{
    return item_type;
}
double Robot::radius() const
{
    return isLoaded() ? 0.53 : 0.45;
}
void Robot::add_target(Index T)
{
    targets.emplace_back(T);
}
void Robot::abort_current_target()
{
    targets.pop_front();
}
void Robot::abort_all_target()
{
    targets.clear();
}
size_t Robot::target_queue_length()
{
    return targets.size();
}
Index Robot::target_queue_head()
{
    return targets.front();
}
Index Robot::target_queue_tail()
{
    return targets.back();
}
std::deque<Index> Robot::get_targets()
{
    return std::deque<Index>{targets.cbegin(), targets.cend()};
}
void Robot::analyze_track()
{
    auto current = get_index(position);
    bool current_excluded = false;

    for (auto i = 0; i < targets.size(); i++)
    {
        auto p = targets[i];
        if (!current_excluded)
        {
            current_excluded = current != p;
            if (!current_excluded) continue;
        }
        if (gameMap->get_distances()(current.y, current.x) == 3)
        {
            auto line = grid_line(current, p);
            auto b = gameMap->near_block(line);
        } else
        {
        }
    }
}
void Robot::set_map(GameMap &map)
{
    gameMap = std::make_shared<GameMap>(map);
}
void Robot::add_path(const Path &path)
{
}
std::tuple<double, double> Robot::see(double field_angle, size_t ray_count, double ray_length, size_t ray_division)
{
    auto dr = ray_length / ray_division;
    auto theta = orientation - field_angle / 2;
    auto dt = field_angle / static_cast<double>(ray_count - 1);
    std::vector<double> ray_sum(ray_count);
    std::vector<double> ray_min(ray_count, 3);
    for (int j = 0; j < ray_count; ++j)
    {
        auto r = radius() + dr;
        Vector2D base{theta};
        for (int i = 0; i < ray_division; ++i)
        {
            auto v = position + r * base;
            auto vi = get_index(v);
            auto d = gameMap->get_distances()(vi.y, vi.x);
            ray_sum[j] += d;
            if (ray_min[j] > d) ray_min[j] = d;
            if (d == 0) break;
            r += dr;
        }
        theta += dt;
    }
    auto direction_i = std::distance(ray_sum.cbegin(), std::max_element(ray_sum.cbegin(), ray_sum.cend()));
    theta = -field_angle / 2 + direction_i * dt;
    return std::make_tuple(theta, ray_sum[direction_i]);
}
