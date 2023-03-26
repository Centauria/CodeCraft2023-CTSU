//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "arguments.h"
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
    pos_angle_matrix.controllers[0].transform = HardSigmoid(-2.0, 6.0);
    pos_angle_matrix.controllers[1].transform = HardSigmoid(-M_PI, M_PI);
    azimuth_integral.memory_limit = 100;
    azimuth_derivative.memory_limit = 10;
    azimuth_derivative.subtract = angle_diff;
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
            azimuth_derivative.clear();
            azimuth_integral.clear();
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
    if (r.norm() > ANTICOL_NEGLECT_R && !obstacles.empty() && !isLoaded())
    {
        auto pvs = std::vector<Object>();
        auto me = *dynamic_cast<Object *>(this);
        std::transform(obstacles.cbegin(), obstacles.cend(), std::back_inserter(pvs), [me](const Object &o) {
            return o - me;
        });
        auto pvs_alert = std::vector<Object>();
        std::copy_if(pvs.cbegin(), pvs.cend(), std::back_inserter(pvs_alert), [](const Object &o) {
            return o.position.norm() <= ANTICOL_ALERT_R;
        });
        if (!pvs_alert.empty())
        {
            std::vector<Point> pvs_alert_pos;
            std::vector<Velocity> pvs_alert_vel;
            for (auto &pv: pvs_alert)
            {
                pvs_alert_pos.emplace_back(pv.position);
                pvs_alert_vel.emplace_back(pv.velocity);
            }
            pvs_alert_pos.emplace_back();
            pvs_alert_vel.emplace_back();
            auto mc_dist = min_distances(pvs_alert_pos);
            auto mv_dist = min_distances(pvs_alert_vel);
            std::for_each(mc_dist.begin(), mc_dist.end(), [](auto &x) {
                x = 1.0 / x / x;
            });
            std::for_each(mv_dist.begin(), mv_dist.end(), [](auto &x) {
                x = 1.0 / x / x;
            });
            auto mass_center = weighed_average(pvs_alert_pos, mc_dist);
            auto mass_velocity = weighed_average(pvs_alert_vel, mv_dist);
            auto w = HardSigmoid(ANTICOL_WHEEL_K * M_PI / mass_center.norm(), 0, M_PI);
            if (ANTICOL_FLEX_WHEEL)
            {
                double beta = angle_diff(orientation, mass_center.theta());
                if (beta < 0) w = -w;
            }
            auto f = 6.0;
            auto collide_eta = mass_center.dot(mass_velocity);
            action_collision = {f, w};
            weight_collision = collide_eta <= 0 ? ANTICOL_WEIGHT_A / (pow(mass_center.norm(), 2) + ANTICOL_WEIGHT_B) : 0;
        }
    }
    auto azimuth = r.theta();
    auto alpha = angle_diff(azimuth, orientation);
    auto p_error = LeakyReLU(r.norm() - DRIVING_P_ERROR_CORRECTION);

    auto a_der = azimuth_derivative.feed(azimuth, delta);
    auto a_cum = azimuth_integral.feed(abs(a_der), delta);
    auto speed_down = HardSigmoid(DRIVING_SPEEDDOWN_A + DRIVING_SPEEDDOWN_B * a_cum, 0.1, 1);

    LOG("logs/position_error.log", string_format("%lf,%lf", p_error, delta))
    LOG("logs/angle_error.log", string_format("%lf,%lf", alpha, delta))
    std::array<double, 2> result = pos_angle_matrix.feed(
            std::array<double, 2>{p_error, alpha},
            delta);
    double f = result[0];
    f = HardSigmoid(f, -2.0, 6.0);
    double w = result[1];
    w = HardSigmoid(w, -M_PI, M_PI);
    f -= (DRIVING_TURN_BREAK * abs(w));
    f *= speed_down;

    LOG("logs/ETA_error.log", string_format("%lf,%lf", ETA(), delta))
    Action action_target{f, w};
    double weight_target = std::max(DRIVING_WEIGHT_A / (pow(r.norm(), 2) + DRIVING_WEIGHT_B), DRIVING_THRESHOLD);

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
Point Robot::target_queue_head()
{
    return targets.front();
}
Point Robot::target_queue_tail()
{
    return targets.back();
}
std::deque<Point> Robot::get_targets()
{
    return std::deque<Point>{targets.cbegin(), targets.cend()};
}