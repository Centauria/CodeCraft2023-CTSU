//
// Created by Centauria V. CHEN on 2023/4/8.
//

#ifndef CODECRAFTSDK_PATH_H
#define CODECRAFTSDK_PATH_H

#include <vector>

#include "Vector2D.h"

class Path
{
public:
    Index &operator[](size_t n);
    Point operator()(size_t n) const;

    [[nodiscard]] double curvature(size_t n, size_t step) const;

    std::vector<Index> data;
};

Path bfs(CVector2D map, Index start, Index end);

std::vector<Path> bfs(CVector2D map, Index start, const std::vector<Index> &ends);

#endif//CODECRAFTSDK_PATH_H
