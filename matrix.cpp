//
// Created by Centauria V. CHEN on 2023/4/7.
//
#include "matrix.h"

CMatrix::CMatrix(size_t rows, size_t cols, bool outbound, char outbound_default)
    : rows(rows), cols(cols), outbound(outbound), outbound_default(outbound_default)
{
    data = std::vector<char>(rows * cols);
}
char &CMatrix::operator()(size_t y, size_t x)
{
    if (y >= rows || x >= cols)
    {
        if (outbound) return outbound_default;
        throw std::out_of_range("Index out of range");
    }
    return data[y * cols + x];
}
CMatrix::operator std::string() const
{
    std::stringstream result;
    result << "[";
    for (int j = 0; j < rows; ++j)
    {
        result << "[";
        for (int i = 0; i < cols; ++i)
        {
            result << +data[j * cols + i] << ", ";
        }
        result << "]," << std::endl;
    }
    result << "]," << std::endl;
    return result.str();
}

DMatrix::DMatrix(size_t rows, size_t cols, bool outbound, double outbound_default) : rows(rows), cols(cols), outbound(outbound), outbound_default(outbound_default)
{
    data = std::vector<double>(rows * cols);
}
DMatrix::DMatrix(size_t rows, size_t cols, double init, bool outbound, double outbound_default) : rows(rows), cols(cols), outbound(outbound), outbound_default(outbound_default)
{
    data = std::vector<double>(rows * cols, init);
}
DMatrix::DMatrix(size_t rows, size_t cols, const std::vector<double> &d, bool outbound, double outbound_default) : rows(rows), cols(cols), outbound(outbound), outbound_default(outbound_default)
{
    data.reserve(rows * cols);
    auto n = std::min(d.size(), rows * cols);
    std::copy(d.cbegin(), d.cbegin() + n, std::back_inserter(data));
}
double &DMatrix::operator()(size_t y, size_t x)
{
    if (y >= rows || x >= cols)
    {
        if (outbound) return outbound_default;
        throw std::out_of_range("Index out of range");
    }
    return data[y * cols + x];
}
std::vector<double> DMatrix::operator*(const std::vector<double> &x)
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
DMatrix::operator std::string() const
{
    std::stringstream result;
    result << "[";
    for (int j = 0; j < rows; ++j)
    {
        result << "[";
        for (int i = 0; i < cols; ++i)
        {
            result << data[j * cols + i] << ", ";
        }
        result << "]," << std::endl;
    }
    result << "]," << std::endl;
    return result.str();
}
DMatrix &DMatrix::operator/=(double x)
{
    std::for_each(data.begin(), data.end(), [x](auto &v) { v /= x; });
    return *this;
}