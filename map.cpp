//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"
Map::Map(size_t width, size_t height)
{
    this->width = width;
    this->height = height;
    data = std::make_unique<char *>(new char[width * height]);
}
