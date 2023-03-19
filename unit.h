//
// Created by Centauria V. CHEN on 2023/3/18.
//

#ifndef CODECRAFTSDK_UNIT_H
#define CODECRAFTSDK_UNIT_H


#include "logging.h"
#include <deque>
struct ET {// Error value with delta time variant
    double e;
    double dt;
};

class CalculationUnit
{
public:
    virtual double feed(double e, double dt) = 0;
};
#endif//CODECRAFTSDK_UNIT_H
