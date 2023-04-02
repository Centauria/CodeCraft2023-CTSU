//
// Created by 蔡恩光 on 2023/4/2.
//

#ifndef CODECRAFTSDK_ROADBLOCKER_H
#define CODECRAFTSDK_ROADBLOCKER_H

#include "map.h"

class RoadBlocker
{
public:
    void block_wide_road(DMatrix &Dmap);
    void block_narrow_road(DMatrix &Dmap);
private:
};


#endif//CODECRAFTSDK_ROADBLOCKER_H
