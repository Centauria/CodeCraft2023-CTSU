//
// Created by Centauria V. CHEN on 2023/3/31.
//

#include "coordinate.h"

Point center(Index index, size_t max_rows, size_t max_cols, double pixel_d)
{
    return {(index.x + 0.5) * pixel_d, (max_rows - index.y - 0.5) * pixel_d};
}