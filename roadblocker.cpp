//
// Created by 蔡恩光 on 2023/4/2.
//

#include "roadblocker.h"
#include "matrix.h"

void RoadBlocker::block_wide_road(DMatrix &Dmap)
{
}
void RoadBlocker::block_narrow_road(DMatrix &Dmap)
{
    DMatrix temp = convolve(Dmap, Index{3, 3}, [](DView &v) -> double {
        if (v(1, 1) < 0) return v(1, 1);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == j && i != 1) continue;
                if (v(i, 0) == 0 && v(j, 2) == 0) return -1;
                if (v(0, i) == 0 && v(2, j) == 0) return -1;
            }
        }
        return v(1, 1);
    });
    DView v{Dmap, {1, 1}, {101, 101}};
    v = temp;
}
