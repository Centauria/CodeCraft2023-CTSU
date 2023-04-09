//
// Created by SamuelCai on 2023/3/26.
//

#ifndef CODECRAFTSDK_ARGSPECS_H
#define CODECRAFTSDK_ARGSPECS_H

#include "arguments.h"

const double grid_length = 0.5;

const uint8_t INGREDIENTS[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
};

inline Argument load_args()
{
    Argument args;
    // TODO: compare map by comparing map string

    return args;
}

#endif//CODECRAFTSDK_ARGSPECS_H
