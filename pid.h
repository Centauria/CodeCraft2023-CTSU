//
// Created by Centauria V. CHEN on 2023/3/12.
//

#ifndef CODECRAFTSDK_PID_H
#define CODECRAFTSDK_PID_H

#include "logging.h"
#include "matrix.h"
#include "unit.h"
#include <cassert>
#include <cstdarg>
#include <cstddef>
#include <deque>
#include <functional>
#include <vector>

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

    std::function<double(double)> transform = [](double x) { return x; };

private:
    std::deque<ET> memory;
    double integral_value = 0.0;// avoid compute integral value every time
    int refresh_freq = 0;       // recompute integral value by summing up
    int refresh_count = 0;

    [[nodiscard]] double integral() const;
    [[nodiscard]] double derivative() const;
    void record(double e, double dt);
};

template<size_t input, size_t output>
class PIDMatrix
{
public:
    PIDMatrix();
    explicit PIDMatrix(Matrix<input, 3> pid);
    explicit PIDMatrix(Matrix<input, output> weight);
    PIDMatrix(Matrix<input, 3> pid, Matrix<input, output> weight);

    std::array<double, output> feed(std::array<double, input> x, double dt);

    void setControllersCoefficient(Matrix<input, 3> pid);
    Matrix<input, 3> getControllersCoefficient();

    void clear();

    Matrix<input, output> w;
    std::vector<PIDController> controllers;
};
template<size_t input, size_t output>
void PIDMatrix<input, output>::clear()
{
    for (int i = 0; i < input; ++i)
    {
        controllers[i].clear();
    }
}
template<size_t input, size_t output>
PIDMatrix<input, output>::PIDMatrix()
{
    for (int i = 0; i < input; ++i)
    {
        controllers.emplace_back();
    }
}
template<size_t input, size_t output>
PIDMatrix<input, output>::PIDMatrix(Matrix<input, 3> pid)
{
    for (int i = 0; i < input; ++i)
    {
        controllers.emplace_back(pid(i, 0), pid(i, 1), pid(i, 2));
    }
}
template<size_t input, size_t output>
PIDMatrix<input, output>::PIDMatrix(Matrix<input, output> weight)
{
    PIDMatrix();
    w = weight;
}
template<size_t input, size_t output>
PIDMatrix<input, output>::PIDMatrix(Matrix<input, 3> pid, Matrix<input, output> weight)
{
    for (int i = 0; i < input; ++i)
    {
        controllers.emplace_back(pid(i, 0), pid(i, 1), pid(i, 2));
    }
    w = weight;
}
template<size_t input, size_t output>
Matrix<input, 3> PIDMatrix<input, output>::getControllersCoefficient()
{
    Matrix<input, 3> pid;
    for (int i = 0; i < input; ++i)
    {
        pid(i, 0) = controllers[i].Kp;
        pid(i, 1) = controllers[i].Ki;
        pid(i, 2) = controllers[i].Kd;
    }
    return pid;
}
template<size_t input, size_t output>
void PIDMatrix<input, output>::setControllersCoefficient(Matrix<input, 3> pid)
{
    for (int i = 0; i < input; ++i)
    {
        controllers[i].Kp = pid(i, 0);
        controllers[i].Ki = pid(i, 1);
        controllers[i].Kd = pid(i, 2);
    }
}
template<size_t input, size_t output>
std::array<double, output> PIDMatrix<input, output>::feed(std::array<double, input> x, double dt)
{
    std::array<double, input> x_pid;
    for (int i = 0; i < input; ++i)
    {
        x_pid[i] = controllers[i].feed(x[i], dt);
    }
    return w * x_pid;
}

#endif//CODECRAFTSDK_PID_H
