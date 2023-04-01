//
// Created by Centauria V. CHEN on 2023/3/21.
//

#ifndef CODECRAFTSDK_MATRIX_H
#define CODECRAFTSDK_MATRIX_H

#include "coordinate.h"
#include "point.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

template<class T>
class AbstractMatrix
{
public:
    virtual T &operator()(size_t y, size_t x) = 0;
    virtual ~AbstractMatrix() = default;
};

template<size_t rows, size_t cols>
class Matrix : public AbstractMatrix<double>
{
public:
    Matrix();
    explicit Matrix(double init);
    explicit Matrix(std::array<double, rows * cols> data);
    double &operator()(size_t y, size_t x) override;
    std::array<double, cols> operator*(std::array<double, rows> x);
    Matrix<rows, cols> operator*(double x);
    Matrix<rows, cols> operator/(double x);
    Matrix<rows, cols> &operator/=(double x);
    Matrix<rows, cols> &transpose();

    explicit operator std::string() const;

private:
    std::array<double, rows * cols> data;
};
template<size_t rows, size_t cols>
Matrix<rows, cols>::operator std::string() const
{
    std::stringstream result;
    result << "[";
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            result << data[j * cols + i] << ", ";
        }
        result << "]," << std::endl;
    }
    result << "]," << std::endl;
    return result.str();
}
template<size_t rows, size_t cols>
Matrix<rows, cols> &Matrix<rows, cols>::transpose()
{
    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            auto k1 = j * cols + i;
            auto k2 = i * cols + j;
            auto temp = data[k1];
            data[k1] = data[k2];
            data[k2] = temp;
        }
    }
    return *this;
}
template<size_t rows, size_t cols>
Matrix<rows, cols> &Matrix<rows, cols>::operator/=(double x)
{
    for (int i = 0; i < rows * cols; ++i)
    {
        data[i] /= x;
    }
    return *this;
}
template<size_t rows, size_t cols>
Matrix<rows, cols> Matrix<rows, cols>::operator/(double x)
{
    std::array<double, rows * cols> result;
    for (int i = 0; i < rows * cols; ++i)
    {
        result[i] = data[i] / x;
    }
    return {result};
}
template<size_t rows, size_t cols>
Matrix<rows, cols> Matrix<rows, cols>::operator*(double x)
{
    std::array<double, rows * cols> result;
    for (int i = 0; i < rows * cols; ++i)
    {
        result[i] = data[i] * x;
    }
    return {result};
}
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

class DMatrix : public AbstractMatrix<double>
{
public:
    DMatrix(size_t rows, size_t cols);
    DMatrix(size_t rows, size_t cols, double init);

    double &operator()(size_t y, size_t x) override;
    std::vector<double> operator*(std::vector<double> x);

    explicit operator std::string() const;

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

DMatrix distance_matrix(const std::vector<Point> &ps);

std::vector<double> min_distances(const std::vector<Point> &ps);

Matrix<3, 3> vandermonde_matrix_inversed(std::array<double, 3> x);

template<class T>
class View : public AbstractMatrix<T>
{
public:
    View(AbstractMatrix<T> *data, Index start, Index end);
    T &operator()(size_t y, size_t x) override;
    explicit operator std::string();

private:
    std::shared_ptr<AbstractMatrix<T>> data;
    Index start{};
    int y_direction;
    int x_direction;
    int rows;
    int cols;
};
template<class T>
View<T>::operator std::string()
{
    std::stringstream result;
    result << "[";
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            result << operator()(j, i) << ", ";
        }
        result << "]," << std::endl;
    }
    result << "]," << std::endl;
    return result.str();
}
template<class T>
T &View<T>::operator()(size_t y, size_t x)
{
    if (y >= rows || x >= cols)
    {
        throw std::out_of_range("Index out of range");
    }
    return (*data)(start.y + y_direction * y, start.x + x_direction * x);
}

// Usage: View(dynamic_cast<AbstractMatrix<double>*>(&m),Index{},Index{});
template<class T>
View<T>::View(AbstractMatrix<T> *data, Index start, Index end)
{
    this->data = static_cast<const std::shared_ptr<AbstractMatrix<double>>>(data);
    this->start = start;
    auto y = static_cast<int>(end.y) - static_cast<int>(start.y);
    rows = abs(y);
    y_direction = y >= 0 ? 1 : -1;
    auto x = static_cast<int>(end.x) - static_cast<int>(start.x);
    cols = abs(x);
    x_direction = x >= 0 ? 1 : -1;
}

#endif//CODECRAFTSDK_MATRIX_H
