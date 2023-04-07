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
std::vector<double> CMatrix::operator*(std::vector<double> x)
{
    return {};
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
            result << data[j * cols + i] << ", ";
        }
        result << "]," << std::endl;
    }
    result << "]," << std::endl;
    return result.str();
}