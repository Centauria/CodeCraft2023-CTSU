//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "robot.h"


Robot::Robot(uint8_t id, double x, double y, double orientation)
{
    this->id = id;
    coordinate = {x, y};
    this->orientation = orientation;
    item_type = 0;
}

Robot::Robot(double x, double y)
{
    coordinate = {x, y};
}

double Robot::ETA()
{
    // Estimated time of arrival
    return 0;
}

void Robot::step()
{
    forward(3);
    rotate(1.5);
}
void Robot::forward(double v)
{
    forward_speed = v;
    std::cout << "forward " << +id << " " << v << std::endl;
}
void Robot::rotate(double w)
{
    rotate_speed = w;
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
void Robot::set_position(const Point &p)
{
    coordinate = p;
    calculate_dynamic();
}
void Robot::set_orientation(double angle)
{
    orientation = angle;
    calculate_dynamic();
}
void Robot::set_target(Point T)
{
    target = T;
}
void Robot::set_obstacle(const std::vector<Point> &obstacles)
{
}
void Robot::calculate_dynamic()
{
    // TODO: decide every dynamic argument
    // forward, rotate
    Vector2D r = target - coordinate;
    auto alpha = angle_diff(r.theta(), orientation);
}
void Robot::calculate_trade()
{
    // TODO: decide every trade argument
    // whether buy, sell, or destroy
}
