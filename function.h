//
// Created by Centauria V. CHEN on 2023/3/13.
//

#ifndef CODECRAFTSDK_FUNCTION_H
#define CODECRAFTSDK_FUNCTION_H

#include <functional>

double ReLU(double x);

double LeakyReLU(double x, double lambda = 0.01);

double HardSigmoid(double x, double lower_limit, double upper_limit);

std::function<double(double)> HardSigmoid(double lower_limit, double upper_limit);

double sinc(double x);

#endif//CODECRAFTSDK_FUNCTION_H
