//
// Created by Centauria V. CHEN on 2023/4/8.
//

#ifndef CODECRAFTSDK_PATH_H
#define CODECRAFTSDK_PATH_H

#include <vector>

#include "Vector2D.h"
#include "matrix.h"

class Path
{
public:
    Index &operator[](size_t n);
    Point operator()(size_t n) const;

    [[nodiscard]] double curvature(size_t n, size_t step) const;
    [[nodiscard]] Vector2D tangent(size_t n, size_t step) const;

    std::vector<Index> data;
};

Path bfs(CMatrix map, Index start, Index end);

std::vector<Path> bfs(CMatrix map, Index start, const std::vector<Index> &ends, int width);

#endif//CODECRAFTSDK_PATH_H
