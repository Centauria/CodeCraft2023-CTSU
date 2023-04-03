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

Path a_star(const GameMap &map, Index start, Index end, int width);

#endif//CODECRAFTSDK_PATH_H
