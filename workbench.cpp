//
// Created by Centauria V. CHEN on 2023/3/11.
//

#include <cstring>

#include "workbench.h"


WorkBench::WorkBench(int16_t type, double x, double y)
{
    this->type = type;
    coordinate = {x, y};
    material_status = 0;
    product_status = false;
    product_frames_remained = -1;
}

WorkBench::WorkBench(int16_t type, double x, double y, int product_frames_remained,
                     int16_t material_status, bool product_status)
{
    this->type = type;
    coordinate = {x, y};
    this->product_frames_remained = product_frames_remained;
    this->material_status = material_status;
    this->product_status = product_status;
}

double WorkBench::ETA()
{
    return 0;
}

bool WorkBench::isFree(int index) const
{
    return static_cast<bool>(material_status & (1 << index));
}

bool WorkBench::isReady() const
{
    return product_status;
}

bool WorkBench::needRawMaterial(int index)
{
    bool arr[10][10];
    memset(arr, false, sizeof(arr));
    arr[4][1] = arr[4][2] = arr[5][1] = arr[5][3] =
    arr[6][2] = arr[6][3] = arr[7][4] = arr[7][5] =
    arr[7][6] = arr[8][7] = true;
    for (int i = 1; i <= 7; i++)
    {
        arr[9][i] = true;
    }
    //原材料方案数组👆
    return arr[type][index];
}