//
// Created by Centauria V. CHEN on 2023/3/10.
//

#ifndef CODECRAFTSDK_ROBOT_H
#define CODECRAFTSDK_ROBOT_H

#include <cstdint>

class Robot
{
public:
    explicit Robot(uint8_t id);
    void step();
    void forward(double v);
    void rotate(double w);
    void buy();
    void sell();
    void destroy();

    // these functions may be useful
    double ETA();

private:
    uint8_t id;
    double forward_speed = 0.0;
    double rotate_speed = 0.0;
};


#endif//CODECRAFTSDK_ROBOT_H
