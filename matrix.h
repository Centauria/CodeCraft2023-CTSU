//
// Created by Centauria V. CHEN on 2023/4/7.
//

#ifndef CODECRAFTSDK_MATRIX_H
#define CODECRAFTSDK_MATRIX_H

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class CMatrix
{
public:
    CMatrix(size_t rows, size_t cols, bool outbound = false, char outbound_default = 0);

    char &operator()(size_t y, size_t x);

    explicit operator std::string() const;

    size_t rows, cols;

private:
    std::vector<char> data;
    bool outbound = false;
    char outbound_default = 0;
};

class DMatrix
{
public:
    DMatrix(size_t rows, size_t cols, bool outbound = false, double outbound_default = 0.0);
    DMatrix(size_t rows, size_t cols, double init, bool outbound = false, double outbound_default = 0.0);
    DMatrix(size_t rows, size_t cols, const std::vector<double> &d, bool outbound = false, double outbound_default = 0.0);

    double &operator()(size_t y, size_t x);
    std::vector<double> operator*(const std::vector<double> &x);
    friend std::vector<double> operator*(const std::vector<double> &x, const DMatrix &m);
    DMatrix &operator/=(double x);

    explicit operator std::string() const;

    size_t rows, cols;

private:
    std::vector<double> data;
    bool outbound = false;
    double outbound_default = 0.0;
};

#endif//CODECRAFTSDK_MATRIX_H
