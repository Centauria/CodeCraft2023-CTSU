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
double Path::curvature(size_t n, size_t step) const
{
    int a_index = int(n - step);
    int c_index = int(n + step);
    while (a_index < 0 || a_index == n || a_index == c_index) a_index++;
    while (c_index >= data.size() || c_index == n || c_index == a_index) c_index--;
    std::vector<int> indexes{a_index, int(n), c_index};
    std::sort(indexes.begin(), indexes.end());
    auto curve = ::curvature(get_point(data[indexes[0]]), get_point(data[indexes[1]]), get_point(data[indexes[2]]));
    return curve;
}
