//
// Created by Centauria V. CHEN on 2023/3/13.
//

#include "function.h"
#include "matrix.h"

#include <array>
#include <cmath>

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

std::function<double(double)> HardSigmoid(double lower_limit, double upper_limit)
{
    return [lower_limit, upper_limit](double x) {
        if (x > upper_limit) return upper_limit;
        if (x < lower_limit) return lower_limit;
        return x;
    };
}

double sinc(double x)
{
    return x == 0 ? 1 : sin(x) / x;
}

double rand_uniform(double lower_limit, double upper_limit)
{
    std::uniform_real_distribution<> dist(lower_limit, upper_limit);
    return dist(E2);
}

std::vector<double> linspace(double lower_limit, double upper_limit, size_t n)
{
    std::vector<double> result(n + 1);
    for (auto i = 0; i <= n; i++)
    {
        result[i] = lower_limit + (upper_limit - lower_limit) * i / static_cast<double>(n);
    }
    return result;
}

std::tuple<double, double> minimize(const std::function<double(double)> &f, double lower_limit, double upper_limit, double precision, size_t slice_per_iter)
{
    while (upper_limit - lower_limit >= precision)
    {
        auto grid = linspace(lower_limit, upper_limit, slice_per_iter);
        std::vector<double> y;
        std::transform(grid.cbegin(), grid.cend(), std::back_inserter(y), f);
        auto min_index = std::min_element(y.begin(), y.end()) - y.begin();
        auto a = min_index - 1, b = min_index + 1;
        if (a < 0) a = 0;
        if (b >= slice_per_iter) b = grid.cend() - grid.cbegin() - 1;
        lower_limit = grid[a];
        upper_limit = grid[b];
    }
    auto x_m = (lower_limit + upper_limit) / 2;
    return std::make_tuple(x_m, f(x_m));
}

std::tuple<double, double> minimize_quad(const std::function<double(double)> &f, double lower_limit, double upper_limit)
{
    std::array<double, 3> x{lower_limit, (lower_limit + upper_limit) / 2, upper_limit};
    auto vandermonde_inv = vandermonde_matrix_inversed(x);
    std::array<double, 3> y{f(x[0]), f(x[1]), f(x[2])};
    auto [a, b, c] = vandermonde_inv.transpose() * y;
    auto axis = -b / a / 2;
    if (axis > upper_limit) return std::make_tuple(upper_limit, f(upper_limit));
    if (axis < lower_limit) return std::make_tuple(lower_limit, f(lower_limit));
    return std::make_tuple(axis, f(axis));
}