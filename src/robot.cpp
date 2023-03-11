//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <iostream>

#include "robot.h"


Robot::Robot(uint8_t id)
{
    this->id = id;
}

double Robot::ETA()
{
    // Estimated time of arrival
    return 0;
}

void Robot::step()
{
    forward(3, 3);
    rotate(1.5);
}
void Robot::forward(double v_x, double v_y)
{
    forward_speed_x = v_x;
    forward_speed_y = v_y;
    std::cout << "forward " << +id << " " << v_x << " " << v_y << std::endl;
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
