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

class GameMap;

double h(Index start, Index end);
std::vector<Index> list_all_neighbors(Index current, int dist);
Path reconstruct_path(Index from[][105], Index start, Index end);
bool check(Index a, DMatrix &Dmap, int width);
Path a_star(GameMap &map, Index start, Index end, int width);
Path grid_line(Index start, Index end);

#endif//CODECRAFTSDK_PATH_H
