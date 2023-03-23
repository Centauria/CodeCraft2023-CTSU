//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

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
    pos_angle_matrix.controllers[0].transform = HardSigmoid(-2.0, 5.0);
    pos_angle_matrix.controllers[1].transform = HardSigmoid(-M_PI, M_PI);
}

double Robot::ETA()
{
    // Estimated time of arrival
    return ETA(std::vector<Point>{targets.cbegin(), targets.cend()});
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
    auto action = calculate_dynamic(delta);
    forward(action.forward);
    rotate(action.rotate);

    if (!targets.empty())
    {
        Point p = targets.front();
        Trade t = calculate_trade();
        if (t != NONE)
        {
            targets.pop_front();
            pos_angle_matrix.clear();
        }
        return std::make_tuple(t, p);
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
void Robot::set_target(Point T)
{
    target = T;
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
    Vector2D r = targets.front() - position;
    Action action_collision{};
    double weight_collision = 0;
    if (r.norm() > 1.0 && !obstacles.empty())
    {
        auto pvs = std::vector<Object>();
        auto me = *dynamic_cast<Object *>(this);
        std::transform(obstacles.cbegin(), obstacles.cend(), std::back_inserter(pvs), [me](const Object &o) {
            return o - me;
        });
        auto pvs_alert = std::vector<Object>();
        std::copy_if(pvs.cbegin(), pvs.cend(), std::back_inserter(pvs_alert), [](const Object &o) {
            return o.position.norm() <= 2.0;
        });
        if (!pvs_alert.empty())
        {
            Point mass_center, mass_velocity;
            for (auto &pv: pvs_alert)
            {
                mass_center += pv.position;
                mass_velocity += pv.velocity;
            }
            mass_center /= (double) (pvs_alert.size() + 1);
            mass_velocity /= (double) (pvs_alert.size() + 1);
            auto beta = angle_diff(orientation, mass_center.theta());
            auto w = M_PI_2;
            if (beta < 0) w = -w;
            auto f = mass_center.norm() > 0.8 ? 5.0 : 0;
            auto collide_eta = mass_center.dot(mass_velocity);
            action_collision = {f, w};
            weight_collision = collide_eta <= 0 ? pow(10, 3 - 2 * mass_center.norm()) : 0;
        }
    }
    auto alpha = angle_diff(r.theta(), orientation);
    auto p_error = LeakyReLU(r.norm() - 0.3);

    LOG("logs/position_error.log", string_format("%lf,%lf", p_error, delta))
    LOG("logs/angle_error.log", string_format("%lf,%lf", alpha, delta))
    std::array<double, 2> result = pos_angle_matrix.feed(
            std::array<double, 2>{p_error, alpha},
            delta);
    double f = result[0];
    //    f = HardSigmoid(f, -2.0, 5.0);
    double w = result[1];
    //    w = HardSigmoid(w, -M_PI, M_PI);
    f += (-0.5 * abs(w));

    LOG("logs/ETA_error.log", string_format("%lf,%lf", ETA(), delta))
    Action action_target{f, w};
    double weight_target = std::max(36.1 / (pow(r.norm(), 2) + 0.1), 1.0);

    Vector2D AC{action_collision.forward, action_collision.rotate};
    Vector2D AT{action_target.forward, action_target.rotate};
    Vector2D A = (AC * weight_collision + AT * weight_target) / (weight_collision + weight_target);
    return {A.x, A.y};
}

Trade Robot::calculate_trade()
{
    if (workbench_id != -1 && (targets.front() - position).norm() < 0.4)
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
void Robot::add_target(Point T)
{
    targets.emplace_back(T);
}
void Robot::abort_current_target()
{
    targets.pop_front();
    pos_angle_matrix.clear();
}
void Robot::abort_all_target()
{
    targets.clear();
    pos_angle_matrix.clear();
}
size_t Robot::target_queue_length()
{
    return targets.size();
}
