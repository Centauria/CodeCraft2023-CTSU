//
// Created by Centauria V. CHEN on 2023/3/21.
//

#include "matrix.h"

#include <exception>

DMatrix distance_matrix(const std::vector<Point> &ps)
{
    auto n = ps.size();
    DMatrix result(n, n);
    for (int j = 0; j < n; ++j)
    {
        for (int i = 0; i < j; ++i)
        {
            auto d = (ps[i] - ps[j]).norm();
            result(i, j) = d;
            result(j, i) = d;
        }
    }
    return result;
}

std::vector<double> min_distances(const std::vector<Point> &ps)
{
    auto n = ps.size();
    std::vector<double> result(n);
    auto d_matrix = distance_matrix(ps);
    for (auto i = 0; i < n; i++)
    {
        for (auto j = 0; j < n && j != i; j++)
        {
            if (result[i] == 0 || d_matrix(i, j) < result[i])
            {
                result[i] = d_matrix(i, j);
            }
        }
    }
    return result;
}
DMatrix::DMatrix(size_t rows, size_t cols)
{
    this->rows = rows;
    this->cols = cols;
    data = std::vector<double>(rows * cols);
}
DMatrix::DMatrix(size_t rows, size_t cols, double init)
{
    this->rows = rows;
    this->cols = cols;
    data = std::vector<double>(rows * cols, init);
}
double &DMatrix::operator()(size_t y, size_t x)
{
    if (y >= rows || x >= cols) throw std::out_of_range("Index out of range");
    return data[y * cols + x];
}
std::vector<double> DMatrix::operator*(std::vector<double> x)
{
    if (x.size() != rows) throw std::invalid_argument("Input vector must have the same dimension as matrix row");
    auto y = std::vector<double>(cols);
    for (int i = 0; i < cols; ++i)
    {
        y[i] = 0;
        for (int j = 0; j < rows; ++j)
        {
            y[i] += data[j * cols + i] * x[j];
        }
    }
    return y;
}
