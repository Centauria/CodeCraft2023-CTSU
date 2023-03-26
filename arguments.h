//
// Created by Centauria V. CHEN on 2023/3/26.
//

#ifndef CODECRAFTSDK_ARGUMENTS_H
#define CODECRAFTSDK_ARGUMENTS_H

#include <cmath>
struct Argument {
    // Robots anti-collision
    double ANTICOL_NEGLECT_R = 1.0;// range(0.0, 4.0)
    double ANTICOL_ALERT_R = 8.0;  // range(4.0, 16.0, 2)
    double ANTICOL_WHEEL_K = 1.0;  // range(0.2, 3.0, 0.2)
    bool ANTICOL_FLEX_WHEEL = true;// ['true','false']
    double ANTICOL_WEIGHT_A = 5.0; // range(0.0, 15.0)
    double ANTICOL_WEIGHT_B = 1.0; // range(0.02, 2.0, 0.02)

    // Robots driving
    double DRIVING_P_ERROR_CORRECTION = 0.3;  // range(0, 0.39, 0.02)
    double DRIVING_SPEEDDOWN_A = 4 * M_PI + 1;// range(5, 25)
    double DRIVING_SPEEDDOWN_B = -1.0;        // range(-10, -0.2, 0.5)
    double DRIVING_TURN_BREAK = 0.5;          // range(0, 1.5, 0.1)
    double DRIVING_WEIGHT_A = 36.1;           // range(0.0, 50.0, 5)
    double DRIVING_WEIGHT_B = 0.1;            // range(0.02, 2.0, 0.02)
    double DRIVING_THRESHOLD = 1.0;           // range(0, 2.0, 0.5)

    double IF_ALREADY_PRODUCING = 17.0;// range(5.0, 25.0, 2)
    double IF_NOTHING_INSIDE = 11.0;   // range(2.0, 17.0, 2)
    double AVERAGING_456 = 4.0;        // range(0,5,1)
    double DONT_FILL_9_WORKBENCH = 6.0;// range(0,8,1)
    double AVOIDING_COLLISION = 1.7;   // [0, 1.7, 2.3]
};
#endif//CODECRAFTSDK_ARGUMENTS_H
