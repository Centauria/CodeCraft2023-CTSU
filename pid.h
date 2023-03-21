//
// Created by Centauria V. CHEN on 2023/3/12.
//

#ifndef CODECRAFTSDK_PID_H
#define CODECRAFTSDK_PID_H

#include "logging.h"
#include "unit.h"
#include <cstddef>
#include <deque>

class PIDController : public CalculationUnit
{
public:
    PIDController();
    PIDController(double p, double i, double d, int refresh_freq = 0, int memory_limit = 1000);

    double feed(double e, double dt) override;
    double feed_if(double e, double dt);
    double Kp = 0.5, Ki = 0.5, Kd = 0.5;
    int memory_limit = 1000;

    size_t memorySize();
    void clear();

private:
    std::deque<ET> memory;
    double integral_value = 0.0;// avoid compute integral value every time
    int refresh_freq = 0;       // recompute integral value by summing up
    int refresh_count = 0;

    [[nodiscard]] double integral() const;
    [[nodiscard]] double derivative() const;
    void record(double e, double dt);
};

#endif//CODECRAFTSDK_PID_H
