//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "function.h"
#include "logging.h"
#include "robot.h"
#include <cstring>
#include <valarray>

Robot::Robot(int16_t id, double x, double y)
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
    this->obstacles = obstacles;
}
Action Robot::calculate_dynamic(double delta)
{
    // TODO: decide every dynamic argument
    // forward, rotate
    Vector2D r = target - coordinate;
    auto alpha = angle_diff(r.theta(), orientation);
    auto p_error = LeakyReLU(r.norm() - 0.3);
    auto f = position_error.feed(p_error, delta);
    f = position_delay.feed(f, delta);
    LOG("logs/position_error.log", string_format("%lf,%lf", p_error, delta))
    auto w = angle_error.feed(alpha, delta);
    LOG("logs/angle_error.log", string_format("%lf,%lf", alpha, delta))
    // obstacles
    if (!obstacles.empty())
    {
        auto distance = std::vector<double>();
        std::transform(obstacles.cbegin(), obstacles.cend(), std::back_inserter(distance), [this](Point p) {
            return (this->coordinate - p).norm();
        });
        auto min_index = std::distance(distance.cbegin(), std::min_element(distance.cbegin(), distance.cend()));
        auto y = obstacle_position_error.feed(distance[min_index], delta);
        if (y > 1.3 * velocity.norm())
        {
            w += 2.0 * asin(1.0 / (1.0 + distance[min_index]));
        }
    }
    return {f, w};
}
void Robot::calculate_trade()
{
    // TODO: decide every trade argument
    // whether buy, sell, or destroy
    return;
}
