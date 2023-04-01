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

class DMatrix : public AbstractMatrix<double>
{
public:
    DMatrix(size_t rows, size_t cols);
    DMatrix(size_t rows, size_t cols, double init);
    DMatrix(size_t rows, size_t cols, const std::vector<double> &d);

    double &operator()(size_t y, size_t x) override;
    std::vector<double> operator*(std::vector<double> x);
    DMatrix &operator/=(double x);

    explicit operator std::string() const;

    size_t rows, cols;

private:
    std::vector<double> data;
};

DMatrix distance_matrix(const std::vector<Point> &ps);

std::vector<double> min_distances(const std::vector<Point> &ps);

DMatrix vandermonde_matrix_inversed(std::array<double, 3> x);

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
        result << "[";
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
