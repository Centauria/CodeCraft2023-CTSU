//
// Created by Centauria V. CHEN on 2023/4/3.
//

#ifndef CODECRAFTSDK_PATH_H
#define CODECRAFTSDK_PATH_H

#include "coordinate.h"
#include "map.h"
#include "matrix.h"

#include <vector>

typedef std::vector<Index> Path;

struct Node {
    double Fscore;
    Index index;
    friend bool operator<(Node a, Node b)
    {
        return a.Fscore > b.Fscore;
    }
};

double h(Index start, Index end);
std::vector<Index> list_all_neighbors(Index current);
std::vector<Index> list_all_neighbors(Index current);
Path reconstruct_path(Index from[][100], Index start, Index end);
bool check(Index a, DMatrix &Dmap);
Path a_star(GameMap &map, Index start, Index end, int width);

#endif//CODECRAFTSDK_PATH_H
