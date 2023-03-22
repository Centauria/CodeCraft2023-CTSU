//
// Created by Centauria V. CHEN on 2023/3/13.
//

#ifndef CODECRAFTSDK_FUNCTION_H
#define CODECRAFTSDK_FUNCTION_H

#include <functional>
#include <random>

double ReLU(double x);

double LeakyReLU(double x, double lambda = 0.01);

double HardSigmoid(double x, double lower_limit, double upper_limit);

std::function<double(double)> HardSigmoid(double lower_limit, double upper_limit);

double sinc(double x);

static std::random_device RD;
static std::mt19937 E2(RD());
double rand_uniform(double lower_limit, double upper_limit);

#endif//CODECRAFTSDK_FUNCTION_H
