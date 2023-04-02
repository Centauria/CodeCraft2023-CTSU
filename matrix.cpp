//
// Created by Centauria V. CHEN on 2023/3/21.
//

#include "matrix.h"

#include <array>
#include <stdexcept>

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
        for (auto j = 0; j < n; j++)
        {
            if (j != i && (result[i] == 0 || d_matrix(i, j) < result[i]))
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
DMatrix::DMatrix(size_t rows, size_t cols, const std::vector<double> &d)
{
    this->rows = rows;
    this->cols = cols;
    data.reserve(rows * cols);
    auto n = std::min(d.size(), rows * cols);
    std::copy(d.cbegin(), d.cbegin() + n, std::back_inserter(data));
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

DMatrix vandermonde_matrix_inversed(std::array<double, 3> x)
{
    double det = (x[0] - x[1]) * (x[0] - x[2]) * (x[1] - x[2]);
    DMatrix v{3, 3, std::vector<double>{x[1] - x[2], (x[2] - x[1]) * (x[2] + x[1]), x[1] * x[2] * (x[1] - x[2]), x[2] - x[0], (x[0] - x[2]) * (x[0] + x[2]), x[2] * x[0] * (x[2] - x[0]), x[0] - x[1], (x[1] - x[0]) * (x[1] + x[0]), x[0] * x[1] * (x[0] - x[1])}};
    v /= det;
    return v;
}

DView &DView::operator=(DMatrix &src)
{
    for (int j = 0; j < rows; ++j)
    {
        for (int i = 0; i < cols; ++i)
        {
            operator()(j, i) = src(j, i);
        }
    }
    return *this;
}

DView::operator std::string()
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

double &DView::operator()(size_t y, size_t x)
{
    if (y >= rows || x >= cols)
    {
        throw std::out_of_range("Index out of range");
    }
    return data->operator()(start.y + y_direction * y, start.x + x_direction * x);
}

// Usage: DView(dynamic_cast<AbstractMatrix<double>*>(&m),Index{},Index{});

DView::DView(DMatrix &data, Index start, Index end) : start(start), data(&data)
{
    auto y = end.y - start.y;
    rows = abs(y);
    y_direction = y >= 0 ? 1 : -1;
    auto x = end.x - start.x;
    cols = abs(x);
    x_direction = x >= 0 ? 1 : -1;
}

DMatrix convolve(DMatrix &src, Index kernel_size, const std::function<double(DView &)> &f)
{
    auto ky = kernel_size.y / 2, kx = kernel_size.x / 2;
    DMatrix result{src.rows - ky * 2, src.cols - kx * 2};
    for (auto j = ky; j < src.rows - ky; ++j)
    {
        for (auto i = kx; i < src.cols - kx; ++i)
        {
            auto view = DView{src, {j - ky, i - kx}, {j + ky + 1, i + kx + 1}};
            result(j - ky, i - kx) = f(view);
        }
    }
    return result;
}