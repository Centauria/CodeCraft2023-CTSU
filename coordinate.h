//
// Created by Centauria V. CHEN on 2023/3/31.
//

#ifndef CODECRAFTSDK_COORDINATE_H
#define CODECRAFTSDK_COORDINATE_H

#include "point.h"
#include <string>

struct Index {
    size_t y;
    size_t x;
    explicit operator std::string() const;
    bool operator==(Index v) const;
};

Point center(Index index, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

Index discrete(Point point, size_t max_rows = 100, size_t max_cols = 100, double pixel_d = 0.5);

#endif//CODECRAFTSDK_COORDINATE_H
