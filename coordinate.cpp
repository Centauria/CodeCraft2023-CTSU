//
// Created by Centauria V. CHEN on 2023/3/31.
//

#include "coordinate.h"

#include <sstream>


Index::operator std::string() const
{
    std::stringstream s;
    s << "<" << x << ", " << y << ">";
    return s.str();
}
bool Index::operator==(Index v) const
{
    return y == v.y && x == v.x;
}
Index Index::operator+(Index rhs) const
{
    return {y + rhs.y, x + rhs.x};
}
Index &Index::operator+=(Index rhs)
{
    y += rhs.y;
    x += rhs.x;
    return *this;
}

Point get_point(Index index, size_t max_rows, size_t max_cols, double pixel_d)
{
    return {(index.x + 0.5) * pixel_d, (max_rows - index.y - 0.5) * pixel_d};
}

Index get_index(Point point, size_t max_rows, size_t max_cols, double pixel_d)
{
    return {static_cast<int>(max_rows - 1 - static_cast<int>(point.y / pixel_d)), static_cast<int>(point.x / pixel_d)};
}
