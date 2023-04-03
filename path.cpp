//
// Created by Centauria V. CHEN on 2023/4/3.
//

#include "path.h"
#include <math.h>
#include <queue>
#include <algorithm>

double h(Index start, Index end)
{
    double dx = start.x - end.x, dy = start.y - end.y;
    return sqrt((dx * dx) + (dy * dy));
}

std::vector<Index> list_all_neighbors(Index current)
{
    std::vector<Index> ans;
    if (current.y > 1) ans.push_back({current.x, current.y - 1});
    if (current.x > 1) ans.push_back({current.x - 1, current.y});
    if (current.y < 100) ans.push_back({current.x, current.y + 1});
    if (current.x < 100) ans.push_back({current.x + 1, current.y});
    return ans;
}

Path reconstruct_path(Index from[][100], Index start, Index end)
{
    Path path;
    Index next = end;
    while (!(from[next.x][next.y] == start))
    {
        path.push_back(next);
        next = from[next.x][next.y];
    }
    path.push_back(next);
    std::reverse(path.begin(), path.end());
    return path;
}

bool check(Index a, DMatrix &Dmap)
{
    if (Dmap(a.y, a.x) == 0) return false;
    if (Dmap(a.y, a.x) >= 2) return true;
    for (int j = a.y - 1; j <= j + 1; j++)
    {
        for (int i = a.x - 1; i <= i + 1; i++)
        {
            if (j == a.y && i == a.x) continue;
            if (Dmap(i, a.x - 1) == 0 && Dmap(j, a.x + 1) == 0) return false;
            if (Dmap(a.y - 1, i) == 0 && Dmap(a.y + 1, j) == 0) return false;
        }
    }
    return true;
}

Path a_star(GameMap &map, Index start, Index end, int width)
{
    std::priority_queue<Node> openSet;
    openSet.push(Node{0, start});
    Index from[100][100];
    double Gscore[100][100];
    DMatrix Dmap = map.get_distances();
    while (openSet.size())
    {
        Index current = openSet.top().index;
        openSet.pop();
        if (current == end)
        {
            return reconstruct_path(from, start, end);
        }
        std::vector<Index> neighbor = list_all_neighbors(current);
        for (auto n: neighbor)
        {
            if (check(n, Dmap))
            {
                continue;
            }
            openSet.push({Gscore[current.x][current.y] + 1 + h(n, end), n});
            from[n.x][n.y] = current;
        }
    }
    return {};
}