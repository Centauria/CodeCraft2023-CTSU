//
// Created by Centauria V. CHEN on 2023/3/31.
//

#ifndef CODECRAFTSDK_COORDINATE_H
#define CODECRAFTSDK_COORDINATE_H

#include "point.h"
#include <string>

struct Index {
    int y;
    int x;
    explicit operator std::string() const;
    bool operator==(Index v) const;
    Index operator+(Index rhs) const;
    Index &operator+=(Index rhs);
};

Point get_point(Index index, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

Index get_index(Point point, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

#endif//CODECRAFTSDK_COORDINATE_H
