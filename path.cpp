//
// Created by Centauria V. CHEN on 2023/4/8.
//
#include "path.h"
#include "function.h"
#include <iostream>
#include <queue>
#include <unordered_set>

Path bfs(CVector2D map, Index start, Index end)
{
}

//write a struct to help me hash my Index
struct WorkbenchHash {
    int y, x;
    WorkbenchHash(int j, int i)
    {
        y = j;
        x = i;
    }
    bool operator==(const WorkbenchHash &w) const
    {
        return y == w.y && x == w.x;
    }
};
class HashFunction
{
public:
    size_t operator()(const WorkbenchHash &p) const
    {
        return (std::hash<int>()(p.y)) ^ (std::hash<int>()(p.x));
    }
};

struct Node {
    double distfromOrigin;
    Index index;
    Index parent;
    //Compare by distance function
    friend bool operator<(Node a, Node b)
    {
        return a.distfromOrigin > b.distfromOrigin;
    }
};

bool besideObstacle(Index &index, CMatrix &map)
{
    //if the node is beside obstacle return true
    for (int j = index.y - 1; j <= index.y + 1; j++)
        for (int i = index.x - 1; i <= index.x + 1; i++)
            if (map(j, i) == 0) return true;
    return false;
}

bool isObstacle(Index &index, CMatrix &map)
{
    //if the node is obstacle return true
    return map(index.y, index.x) == 0;
}

//这个function很有可能有bug需要谨慎处理！！！！！！
bool accessible(Index index, CMatrix &map, int width)
{
    //if the node is accessible return true else return false
    if (map(index.y, index.x) == 0) return false;//false 不能过
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if (i == j && i != 0) continue;
            if (map(index.y + i, index.x - 1) == 0 && map(index.y + j, index.x + 1) == 0) return false;
            if (map(index.y - 1, index.x + i) == 0 && map(index.y + 1, index.x + j) == 0) return false;
        }
    }
    if (width == 2) return true;
    for (int i = -2; i <= 2; i++)
    {
        if (i == 0) continue;
        //y轴开搞
        //判断是否越界
        if (map(index.y + i, index.x) == 0)
        {//如果十字上的点是障碍物的话就：
            if (i < 0)
            {
                if (map(index.y + 3 + i, index.x - 1) == 0 || map(index.y + 3 + i, index.x) == 0 || map(index.y + 3 + i, index.x + 1) == 0) return false;
            } else
            {
                if (map(index.y - 3 + i, index.x - 1) == 0 || map(index.y - 3 + i, index.x) == 0 || map(index.y - 3 + i, index.x + 1) == 0) return false;
            }
        }
        //x轴开搞
        if (map(index.y, index.x + i) == 0)
        {
            if (i < 0)
            {
                if (map(index.y - 1, index.x + 3 + i) == 0 || map(index.y, index.x + 3 + i) == 0 || map(index.y + 1, index.x + 3 + i) == 0) return false;
            } else
            {
                if (map(index.y - 1, index.x - 3 + i) == 0 || map(index.y, index.x - 3 + i) == 0 || map(index.y + 1, index.x - 3 + i) == 0) return false;
            }
        }
    }
    return true;
}

//感觉这部分问题很大找陈哥确认一下！！！！！！！！
Path reconstruct_path(Index from[][105], Index start, Index end)
{
    Path path;
    Index next = end;
    while (from[next.y][next.x].y != start.y || from[next.y][next.x].x != start.x)
    {
        path.data.push_back(next);
        next = from[next.y][next.x];
    }
    path.data.push_back(next);
    std::reverse(path.data.begin(), path.data.end());
    return path;
}

std::vector<Path> bfs(CMatrix map, Index start, const std::vector<Index> &ends, int width)
{
    //add all workbenches into a set
    std::unordered_set<WorkbenchHash, HashFunction> workbench_set;
    for (auto &w: ends)
    {
        workbench_set.insert({w.y, w.x});
    }
    std::vector<Path> ans;
    std::priority_queue<Node> pq;
    bool visited[105][105];
    memset(visited, false, sizeof(visited));
    Index from[105][105];
    //push the first node
    pq.push({0, start});
    //set start as visited
    visited[start.y][start.y] = true;
    std::cerr << ends.size() << std::endl;
    while (!pq.empty())
    {
        //get the current node
        Node cur = pq.top();
        pq.pop();
        //check if visited: continue;
        if (visited[cur.index.y][cur.index.x]) continue;
        // mark the current node as visited!
        visited[cur.index.y][cur.index.x] = true;
        // set from to its parent
        from[cur.index.y][cur.index.x] = cur.parent;
        //reconstruct path if I have found a WorkbenchHash
        if (workbench_set.count({cur.index.y, cur.index.x}))
        {
            ans.push_back(reconstruct_path(from, start, cur.index));
        }

        //if I have found all ends: break;
        if (ans.size() == ends.size()) return ans;
        //list all the neighbors that needs to be extended !!! don't add nodes which have already been visited
        std::vector<Index> neighbors;
        for (int j = cur.index.y - 1; j <= cur.index.y + 1; j++)
            for (int i = cur.index.x - 1; i <= cur.index.x + 1; i++)
                if (!visited[j][i]) neighbors.emplace_back(j, i);
        //extend the nodes
        for (auto &neighbor: neighbors)
        {
            //make sure the node we extend is visitable 👇
            if (isObstacle(neighbor, map) || !accessible(neighbor, map, width))
            {
                //if the node out of bound: continue out of bound会自动显示是obstacle所以不用管他
                //if the node is obstacle: continue
                //if the node is not accessible: continue
                continue;
            }
            //node.distfromOrigin = cur.distfromOrigin + 1 or 1.414 + 0 or 1
            double dist = neighbor.y != cur.index.y && neighbor.x != cur.index.x ? M_SQRT2 : 1;
            // offset = 1 if it's close to obstacle
            // else: offset = 0
            int offset = besideObstacle(neighbor, map);//先这么写之后记得回来改
            pq.push({cur.distfromOrigin + dist + offset, neighbor, cur.index});
        }
    }
    std::cerr << "以下为报错信息" << std::endl;
    for (int j = 0; j < 100; j++)
    {
        for (int i = 0; i < 100; i++)
        {
            std::cerr << visited[j][i];
        }
        std::cerr << std::endl;
    }
    std::cerr << "!!!Something went wrong: path.cpp/function: std::vector<Path> bfs()!!!" << std::endl;
    return ans;
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
Vector2D Path::tangent(size_t n, size_t step) const
{
    auto forward_index = std::min(data.size() - 1, n + step);
    Point a, c;
    if (forward_index == n)
    {
        auto backward_index = std::max(0UL, n - step);
        a = operator()(n);
        c = operator()(backward_index);
    } else
    {
        a = operator()(forward_index);
        c = operator()(n);
    }
    return (a - c).normalize();
}