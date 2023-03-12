//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "robot.h"


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

void Robot::step()
{
    auto action = calculate_dynamic();
    forward(action.forward);
    rotate(action.rotate);

    calculate_trade();
}
void Robot::forward(double v)
{
    std::cout << "forward " << +id << " " << v << std::endl;
}
void Robot::rotate(double w)
{
    std::cout << "rotate " << +id << " " << w << std::endl;
}
void Robot::buy()
{
    std::cout << "buy " << +id << std::endl;
}
void Robot::sell()
{
    std::cout << "sell " << +id << std::endl;
}
void Robot::destroy()
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
Action Robot::calculate_dynamic()
{
    // TODO: decide every dynamic argument
    // forward, rotate
    Vector2D r = target - coordinate;
    auto alpha = angle_diff(r.theta(), orientation);
    auto f = position_error.feed(r.norm(), 1.0 / 50);
    auto w = angle_error.feed(alpha, 1.0 / 50);
    return {f, w};
}
void Robot::calculate_trade()
{
    // TODO: decide every trade argument
    // whether buy, sell, or destroy
}
