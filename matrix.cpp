//
// Created by Centauria V. CHEN on 2023/3/21.
//

#include "matrix.h"

#include <exception>

std::vector<double> min_distances(std::vector<Point> ps)
{
    std::vector<double> result(ps.size());
    for (auto i = 0; i < ps.size(); i++)
    {
    }
    return std::vector<double>{};
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
