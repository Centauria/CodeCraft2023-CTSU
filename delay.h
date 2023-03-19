//
// Created by Centauria V. CHEN on 2023/3/18.
//

#ifndef CODECRAFTSDK_DELAY_H
#define CODECRAFTSDK_DELAY_H

#include "unit.h"
#include <deque>

class Delay : public CalculationUnit
{
public:
    explicit Delay(double t);

    double feed(double e, double dt) override;

private:
    double delay_time;
    double memory_time = 0.0;
    volatile double last_e = 0;
    std::deque<ET> memory;
};

#endif//CODECRAFTSDK_DELAY_H
