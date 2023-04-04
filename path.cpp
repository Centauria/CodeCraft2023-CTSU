//
// Created by Centauria V. CHEN on 2023/4/3.
//

#include "path.h"
#include <algorithm>
#include <cstring>
#include <math.h>
#include <queue>

struct Node {
    double Fscore;
    Index index;
    friend bool operator<(Node a, Node b)
    {
        return a.Fscore > b.Fscore;
    }
};

double h(Index start, Index end)
{
    double dx = start.x - end.x, dy = start.y - end.y;
    return sqrt((dx * dx) + (dy * dy));
}

std::vector<Index> list_all_neighbors(Index current, int dist)
{
    std::vector<Index> ans;
    if (current.y > 1) ans.push_back({current.y - 1, current.x});
    if (current.x > 1) ans.push_back({current.y, current.x - 1});
    if (current.y < 100) ans.push_back({current.y + 1, current.x});
    if (current.x < 100) ans.push_back({current.y, current.x + 1});
    if (dist < 3) return ans;
    if (current.y > 1 && current.x > 1) ans.push_back({current.y - 1, current.x - 1});
    if (current.y < 100 && current.x < 100) ans.push_back({current.y + 1, current.x + 1});
    if (current.y > 1 && current.x < 100) ans.push_back({current.y - 1, current.x + 1});
    if (current.y < 100 && current.x > 1) ans.push_back({current.y + 1, current.x - 1});
    return ans;
}

Path reconstruct_path(Index from[][105], Index start, Index end)
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

bool check(Index a, DMatrix &Dmap, int width)
{
    if (Dmap(a.y, a.x) == 0) return false;
    if (Dmap(a.y, a.x) >= 2) return true;
    if (width == 2)
    {
        for (int j = a.y - 1; j <= j + 1; j++)
        {
            for (int i = a.x - 1; i <= i + 1; i++)
            {
                if (j == a.y && i == a.x) continue;
                if (Dmap(i, a.x - 1) == 0 && Dmap(j, a.x + 1) == 0) return false;
                if (Dmap(a.y - 1, i) == 0 && Dmap(a.y + 1, j) == 0) return false;
            }
        }
    } else
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int i = -1; i <= 1; i++)
            {
                if (Dmap(a.y + j, a.x + i) > 0) continue;
                //剩下的Dmap(j, i)都是墙体
                for (int jj = -1; jj <= 1; jj++)
                {
                    for (int ii = -1; ii <= 1; ii++)
                    {
                        if (i == -1 && a.x - i + ii <= 99)
                        {
                            if (Dmap(a.y - i + jj, a.x - i + ii) == 0 && ii - i - i <= 1 && jj - j - i <= 1) return false;
                        } else if (i == 1 && a.x - i + ii >= 2)
                        {
                            if (Dmap(a.y - i + jj, a.x - i + ii) == 0 && ii - i - i <= 1 && jj - j - i <= 1) return false;
                        }
                        if (j == -1 && a.y - j + jj <= 99)
                        {
                            if (Dmap(a.y - j + jj, a.x - j + ii) == 0 && ii - i - j <= 1 && jj - j - j <= 1) return false;
                        } else if (j == 1 && a.y - j + jj >= 2)
                        {
                            if (Dmap(a.y - j + jj, a.x - j + ii) == 0 && ii - i - j <= 1 && jj - j - j <= 1) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

Path a_star(GameMap &map, Index start, Index end, int width)
{
    std::priority_queue<Node> openSet;
    openSet.push(Node{0, start});
    Index from[105][105];
    double Gscore[105][105];
    bool visited[105][105];
    DMatrix Dmap = map.get_distances();
    memset(visited, false, sizeof(visited));
    visited[start.x][start.y] = true;
    while (openSet.size())
    {
        Index current = openSet.top().index;
        openSet.pop();
        if (current == end)
        {
            return reconstruct_path(from, start, end);
        }
        std::vector<Index> neighbor = list_all_neighbors(current, Dmap(current.y, current.x));
        for (auto n: neighbor)
        {
            if (!check(n, Dmap, width) || visited[n.x][n.y]) continue;
            openSet.push({Gscore[current.x][current.y] + 1 + h(n, end), n});
            from[n.x][n.y] = current;
            visited[n.x][n.y] = true;
        }
    }
    return {};
}

Path grid_line(Index start, Index end)
{
    Path line;
    Index z;
    int x_end, y_end, x_dir_flag, y_dir_flag;

    auto dx = abs(end.x - start.x);
    auto dy = abs(end.y - start.y);
    if (dy <= dx)
    {
        auto d = 2 * dy - dx;
        auto inc1 = 2 * dy;
        auto inc2 = 2 * (dy - dx);
        if (start.x > end.x)
        {
            z = end;
            y_dir_flag = -1;
            x_end = start.x;
        } else
        {
            z = start;
            y_dir_flag = 1;
            x_end = end.x;
        }
        line.emplace_back(z);
        if ((end.y - start.y) * y_dir_flag > 0)
        {
            while (z.x < x_end)
            {
                z.x++;
                if (d < 0)
                {
                    d += inc1;
                } else
                {
                    z.y++;
                    d += inc2;
                }
                line.emplace_back(z);
            }
        } else
        {
            while (z.x < x_end)
            {
                z.x++;
                if (d < 0)
                {
                    d += inc1;
                } else
                {
                    z.y--;
                    d += inc2;
                }
                line.emplace_back(z);
            }
        }
    } else
    {
        auto d = 2 * dx - dy;
        auto inc1 = 2 * dx;
        auto inc2 = 2 * (dx - dy);
        if (start.y > end.y)
        {
            z = end;
            x_dir_flag = -1;
            y_end = start.y;
        } else
        {
            z = start;
            x_dir_flag = 1;
            y_end = end.y;
        }
        line.emplace_back(z);
        if ((end.x - start.x) * x_dir_flag > 0)
        {
            while (z.y < y_end)
            {
                z.y++;
                if (d < 0)
                {
                    d += inc1;
                } else
                {
                    z.x++;
                    d += inc2;
                }
                line.emplace_back(z);
            }
        } else
        {
            while (z.y < y_end)
            {
                z.y++;
                if (d < 0)
                {
                    d += inc1;
                } else
                {
                    z.x--;
                    d += inc2;
                }
                line.emplace_back(z);
            }
        }
    }
    if (start != line[0])
    {
        auto half = line.size() / 2;
        for (size_t i = 0, j = line.size() - 1; i < half; i++, j--)
        {
            auto v = line[i];
            line[i] = line[j];
            line[j] = v;
        }
    }
    return line;
}
