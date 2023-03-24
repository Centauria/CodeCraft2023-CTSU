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

template<class T>
T weighed_average(const std::vector<T> &data, std::vector<double> weight)
{
    if (weight.empty())
    {
        for (int i = 0; i < data.size(); ++i)
        {
            weight.emplace_back(1);
        }
    }
    T value;
    for (T t: data)
    {
        value += t;
    }
    value /= data.size();
    return value;
}

#endif//CODECRAFTSDK_FUNCTION_H
