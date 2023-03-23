//
// Created by Centauria V. CHEN on 2023/3/21.
//

#ifndef CODECRAFTSDK_MATRIX_H
#define CODECRAFTSDK_MATRIX_H

#include "point.h"
#include <memory>
#include <vector>

template<size_t rows, size_t cols>
class Matrix
{
public:
    Matrix();
    explicit Matrix(double init);
    explicit Matrix(std::array<double, rows * cols> data);
    double &operator()(size_t y, size_t x);
    std::array<double, cols> operator*(std::array<double, rows> x);

private:
    std::array<double, rows * cols> data;
};
template<size_t rows, size_t cols>
Matrix<rows, cols>::Matrix()
{
    data.fill(0);
}
template<size_t rows, size_t cols>
Matrix<rows, cols>::Matrix(double init)
{
    for (size_t i{0}; i < rows * cols; i++)
    {
        data[i] = init;
    }
}
template<size_t rows, size_t cols>
Matrix<rows, cols>::Matrix(std::array<double, rows * cols> data)
{
    this->data = data;
}
template<size_t rows, size_t cols>
std::array<double, cols> Matrix<rows, cols>::operator*(std::array<double, rows> x)
{
    auto y = std::array<double, cols>();
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
template<size_t rows, size_t cols>
double &Matrix<rows, cols>::operator()(size_t y, size_t x)
{
    return data[y * cols + x];
}

class DMatrix
{
public:
    DMatrix(size_t rows, size_t cols);
    DMatrix(size_t rows, size_t cols, double init);

    double &operator()(size_t y, size_t x);
    std::vector<double> operator*(std::vector<double> x);

    size_t rows, cols;

private:
    std::vector<double> data;
};

template<size_t n>
Matrix<n, n> distance_matrix(std::array<Point, n> ps)
{
    Matrix<n, n> result;
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

std::vector<double> min_distances(std::vector<Point> ps);

#endif//CODECRAFTSDK_MATRIX_H
