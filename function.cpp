//
// Created by Centauria V. CHEN on 2023/3/13.
//

#include "function.h"

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

Point get_point(Index index, size_t max_rows, size_t max_cols, double pixel_d)
{
    return {(index.x + 0.5) * pixel_d, (max_rows - index.y - 0.5) * pixel_d};
}

Index get_index(Point point, size_t max_rows, size_t max_cols, double pixel_d)
{
    return {static_cast<int>(max_rows - 1 - static_cast<int>(point.y / pixel_d)), static_cast<int>(point.x / pixel_d)};
}

double angle_diff(double a, double b)
{
    /*
     * 计算从角度b转向角度a所需最小角度（弧度制）
     * 逆时针为正，顺时针为负
     */
    double diff = a - b;
    int n = static_cast<int>(diff / M_PI);
    return diff - n * M_PI * 2;
}