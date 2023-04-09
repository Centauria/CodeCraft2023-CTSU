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
    [[nodiscard]] double distance() const;

    std::vector<Index> data;
};

//write a struct to help me hash my Index
class HashFunction
{
public:
    size_t operator()(const CVector2D &v) const
    {
        return (std::hash<int>()(v.y)) ^ (std::hash<int>()(v.x));
    }
};

std::vector<Path> bfs(CMatrix map, Index start, const std::vector<Index> &ends, int width);

bool accessible(Index index, CMatrix &map, int width);
#endif//CODECRAFTSDK_PATH_H
