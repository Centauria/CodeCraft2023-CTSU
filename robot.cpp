//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "function.h"
#include "logging.h"
#include "robot.h"
#include "string_util.h"
#include <cmath>
#include <valarray>

Robot::Robot(int16_t id, double x, double y) : Object(Vector2D{x, y}, Vector2D{})
{
    this->id = id;
    this->orientation = 0.0;
    item_type = 0;
    eta_error.memory_limit = 1000;
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

void Robot::step(double delta)
{
    auto action = calculate_dynamic(delta);
    forward(action.forward);
    rotate(action.rotate);

    if (calculate_trade()) targets.pop_front();
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
    eta_error.clear();
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
    // TODO: decide every dynamic argument
    // forward, rotate
    if (targets.empty())
    {
        return {};
    }
    Vector2D r = targets.front() - position;
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

    auto e = eta_error.feed(ETA(), delta);
    LOG("logs/ETA_error.log", string_format("%lf,%lf", ETA(), delta))
    f += e;
    // obstacles
    if (!obstacles.empty())
    {
        auto pvs = std::vector<Object>();
        auto me = *dynamic_cast<Object *>(this);
        std::transform(obstacles.cbegin(), obstacles.cend(), std::back_inserter(pvs), [me](const Object &o) {
            return o - me;
        });
        for (auto &pv: pvs)
        {
            auto d = pv.position.norm();
            if (d <= 8.0)
            {
                auto v = pv.velocity.norm();
                auto beta = pv.position.dot(pv.velocity);
                auto cosine = beta / d / v;
                auto sine = 1.0 / d;
                if (beta < 0 && cosine * cosine + sine * sine > 1)
                {
                    if (d <= 2.0)
                    {
                        auto p_theta = acos(pv.velocity.dot(Vector2D{orientation}) / v);
                        f *= (HardSigmoid(M_PI - p_theta, asin(sine), 1));
                        f = HardSigmoid(f, 0.0, 5.0);
                    } else if (d <= 8.0)
                    {
                        auto w_diff = 6.0 / (1.1 + d);
                        w += w_diff;
                    }
                }
            }
        }
    }
    return {f, w};
}
bool Robot::calculate_trade()
{
    if (workbench_id != -1 && (targets.front() - position).norm() < 0.4)
    {
        if (isLoaded())
        {
            sell();
        } else
        {
            buy();
        }
        return true;
    }
    return false;
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
}
void Robot::abort_all_target()
{
    targets.clear();
}
size_t Robot::target_queue_length()
{
    return targets.size();
}
