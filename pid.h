//
// Created by Centauria V. CHEN on 2023/3/12.
//

#ifndef CODECRAFTSDK_PID_H
#define CODECRAFTSDK_PID_H

#include "logging.h"
#include <deque>

struct ET {// Error value with delta time variant
    double e;
    double dt;
};

class PIDController
{
public:
    PIDController();
    PIDController(double p, double i, double d, int refresh_freq = 0, int memory_limit = 1000);

    double feed(double e, double dt);
    double feed_if(double e, double dt);
    double Kp = 0.5, Ki = 0.5, Kd = 0.5;

private:
    std::deque<ET> memory;
    int memory_limit = 1000;
    double integral_value = 0.0;// avoid compute integral value every time
    int refresh_freq = 0;       // recompute integral value by summing up
    int refresh_count = 0;

    [[nodiscard]] double integral() const;
    [[nodiscard]] double derivative() const;
    void record(double e, double dt);
};

#endif//CODECRAFTSDK_PID_H
