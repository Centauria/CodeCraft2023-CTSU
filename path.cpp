//
// Created by Centauria V. CHEN on 2023/4/8.
//
#include "path.h"
#include "function.h"

Path bfs(CVector2D map, Index start, Index end)
{
}

std::vector<Path> bfs(CVector2D map, Index start, const std::vector<Index> &ends)
{
}
Index &Path::operator[](size_t n)
{
    return data[n];
}
Point Path::operator()(size_t n) const
{
    return get_point(data[n]);
}
