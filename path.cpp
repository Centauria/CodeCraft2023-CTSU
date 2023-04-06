//
// Created by Centauria V. CHEN on 2023/4/3.
//

#include "path.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
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
    //    return abs(dx)+abs(dy);
}

std::vector<Index> list_all_neighbors(Index current, int dist)
{
    std::vector<Index> ans;
    if (current.y > 0) ans.emplace_back(current.y - 1, current.x);
    if (current.x > 0) ans.emplace_back(current.y, current.x - 1);
    if (current.y < 99) ans.emplace_back(current.y + 1, current.x);
    if (current.x < 99) ans.emplace_back(current.y, current.x + 1);
    if (dist < 3) return ans;
    if (current.y > 0 && current.x > 0) ans.emplace_back(current.y - 1, current.x - 1);
    if (current.y < 99 && current.x < 99) ans.emplace_back(current.y + 1, current.x + 1);
    if (current.y > 0 && current.x < 100) ans.emplace_back(current.y - 1, current.x + 1);
    if (current.y < 99 && current.x > 0) ans.emplace_back(current.y + 1, current.x - 1);
    return ans;
}

Path reconstruct_path(int from_x[][105], int from_y[][105], Index start, Index end)
{
    std::cerr << "reconstructing" << std::endl;
    Path path;
    Index next = end;
    while (from_x[next.x][next.y] != start.x||from_y[next.x][next.y] != start.y)
    {
        path.push_back(next);
        next.x = from_x[next.x][next.y];
        next.y = from_y[next.x][next.y];
    }
    path.push_back(next);
    std::reverse(path.begin(), path.end());
    return path;
}

bool check(Index a, DMatrix &Dmap, int width)
{
    if (Dmap(a.y, a.x) == 0) return false;//false 不能过
    if (Dmap(a.y, a.x) >= 2) return true; //true 能过
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if (i == j && i != 0) continue;
            if (Dmap(a.y + i, a.x - 1) == 0 && Dmap(a.y + j, a.x + 1) == 0) return false;
            if (Dmap(a.y - 1, a.x + i) == 0 && Dmap(a.y + 1, a.x + j) == 0) return false;
        }
    }
    if (width < 3) return true;
    for (int i = -2; i <= 2; i++)
    {
        if (i == 0) continue;
        //y轴开搞
        //判断是否越界
        if (0 <= a.y + i && a.y + i <= 99)
        {
            if (Dmap(a.y + i, a.x) == 0)
            {//如果十字上的点是障碍物的话就：
                if (i < 0)
                {
                    if (Dmap(a.y + 3, a.x - 1) == 0 || Dmap(a.y + 3, a.x) == 0 || Dmap(a.y + 3, a.x + 1) == 0) return false;
                } else
                {
                    if (Dmap(a.y - 3, a.x - 1) == 0 || Dmap(a.y - 3, a.x) == 0 || Dmap(a.y - 3, a.x + 1) == 0) return false;
                }
            }
        }
        //x轴开搞
        if (0 <= a.x + i && a.x + i <= 99)
        {
            if (Dmap(a.y, a.x + i) == 0)
            {
                if (i < 0)
                {
                    if (Dmap(a.y - 1, a.x + 3) == 0 || Dmap(a.y, a.x + 3) == 0 || Dmap(a.y + 1, a.x + 3) == 0) return false;
                } else
                {
                    if (Dmap(a.y - 1, a.x - 3) == 0 || Dmap(a.y, a.x - 3) == 0 || Dmap(a.y + 1, a.x - 3) == 0) return false;
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
    int from_x[105][105], from_y[105][105];
    double Gscore[105][105];
    for (int i = 0; i < 105; i++)
    {
        for (int j = 0; j < 105; j++)
        {
            Gscore[i][j] = 999999;
        }
    }
    Gscore[start.x][start.y] = 0;
    DMatrix Dmap = map.get_distances();
    while (!openSet.empty())
    {
        Index current = openSet.top().index;
        openSet.pop();
        if (current == end)
        {
            return reconstruct_path(from_x, from_y, start, end);
        }
        std::vector<Index> neighbor = list_all_neighbors(current, Dmap(current.y, current.x));
        for (auto n: neighbor)
        {
            if (!check(n, Dmap, width))
            {
                continue;
            }
            if (Gscore[n.x][n.y] > Gscore[current.x][current.y] + 1)
            {
                if(n.x != current.x && n.y != current.y)Gscore[n.x][n.y] = Gscore[current.x][current.y] + M_SQRT2;
                else Gscore[n.x][n.y] = Gscore[current.x][current.y] + 1;
                openSet.push({Gscore[n.x][n.y] + h(n, end), n});
                from_x[n.x][n.y] = current.x;
                from_y[n.x][n.y] = current.y;
            }
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
