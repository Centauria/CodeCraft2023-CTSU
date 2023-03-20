//
// Created by Centauria V. CHEN on 2023/3/13.
//

#include "function.h"

double ReLU(double x)
{
    return x > 0 ? x : 0;
}

double LeakyReLU(double x, double lambda)
{
    return x > 0 ? x : lambda * x;
}

double HardSigmoid(double x, double lower_limit, double upper_limit)
{
    if (x > upper_limit) return upper_limit;
    if (x < lower_limit) return lower_limit;
    return x;
}