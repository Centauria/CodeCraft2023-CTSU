//
// Created by Centauria V. CHEN on 2023/3/31.
//

#ifndef CODECRAFTSDK_COORDINATE_H
#define CODECRAFTSDK_COORDINATE_H

#include "point.h"
#include <string>

struct Index {
    int y = 0;
    int x = 0;
    Index() = default;
    Index(int y, int x) : y(y), x(x){};
    explicit operator std::string() const;
    inline bool operator==(Index v) const;
    inline bool operator!=(Index v) const;
    Index operator+(Index rhs) const;
    Index operator-(Index rhs) const;
    Index &operator+=(Index rhs);
};
inline bool Index::operator==(Index v) const
{
    return y == v.y && x == v.x;
}
inline bool Index::operator!=(Index v) const
{
    return y != v.y || x != v.x;
}

Point get_point(Index index, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

Index get_index(Point point, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

#endif//CODECRAFTSDK_COORDINATE_H
