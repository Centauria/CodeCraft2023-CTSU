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

class DView;

class DMatrix : public AbstractMatrix<double>
{
public:
    DMatrix(size_t rows, size_t cols, bool outbound_supported = false, double outbound_default = 0.0);
    DMatrix(size_t rows, size_t cols, double init, bool outbound_supported = false, double outbound_default = 0.0);
    DMatrix(size_t rows, size_t cols, const std::vector<double> &d, bool outbound_supported = false, double outbound_default = 0.0);

    double &operator()(size_t y, size_t x) override;
    std::vector<double> operator*(std::vector<double> x);
    DMatrix &operator/=(double x);

    explicit operator std::string() const;

    size_t rows, cols;

private:
    std::vector<double> data;
    bool outbound_support = false;
    double default_value = 0.0;
};

DMatrix distance_matrix(const std::vector<Point> &ps);

std::vector<double> min_distances(const std::vector<Point> &ps);

DMatrix vandermonde_matrix_inversed(std::array<double, 3> x);

class DView : public AbstractMatrix<double>
{
public:
    DView(DMatrix &data, Index start, Index end);
    double &operator()(size_t y, size_t x) override;
    DView &operator=(DMatrix &src);
    explicit operator std::string();

private:
    DMatrix *data;
    Index start;
    int y_direction;
    int x_direction;
    int rows;
    int cols;
};

DMatrix convolve(DMatrix &src, Index kernel_size, const std::function<double(DView &)> &f, bool same = false);

#endif//CODECRAFTSDK_MATRIX_H
