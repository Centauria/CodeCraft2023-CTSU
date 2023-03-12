//
// Created by Centauria V. CHEN on 2023/3/12.
//

#ifndef CODECRAFTSDK_PID_H
#define CODECRAFTSDK_PID_H

#include <deque>

struct ET {// Error value with delta time variant
    double e;
    double dt;
};

class PIDController
{
public:
    PIDController();
    PIDController(double p, double i, double d);

    double feed(double e, double dt);

private:
    double Kp = 0.5, Ki = 0.5, Kd = 0.5;
    std::deque<ET> memory;
    int memory_limit = 1000;

    double integral();
    double derivative();
    void record(double e, double dt);
};

#endif//CODECRAFTSDK_PID_H
