//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "log/logging.h"
#include "math/function.h"
#include "robot.h"
#include "util/string.h"

Robot::Robot(uint8_t id, double x, double y)
{
    this->id = id;
    coordinate = {x, y};
    this->orientation = 0.0;
    item_type = 0;
}

double Robot::ETA()
{
    // Estimated time of arrival
    return 0;
}

void Robot::step(double delta)
{
    auto action = calculate_dynamic(delta);
    forward(action.forward);
    rotate(action.rotate);

    calculate_trade();
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
void Robot::set_obstacle(const std::vector<Point> &obstacles)
{
}
Action Robot::calculate_dynamic(double delta)
{
    // TODO: decide every dynamic argument
    // forward, rotate
    Vector2D r = target - coordinate;
    auto alpha = angle_diff(r.theta(), orientation);
    auto p_error = LeakyReLU(r.norm() - 0.8);
    auto f = position_error.feed(p_error, delta);
    LOG("logs/position_error.log", string_format("%lf,%lf", p_error, delta))
    auto w = angle_error.feed(alpha, delta);
    LOG("logs/angle_error.log", string_format("%lf,%lf", alpha, delta))
    return {f, w};
}
void Robot::calculate_trade()
{
    // TODO: decide every trade argument
    // whether buy, sell, or destroy
}
