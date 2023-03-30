//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"

Map::Map(size_t height, size_t width)
{
    this->width = width;
    this->height = height;
    data = std::string(width * height, '.');
}
void Map::append_line(const std::string &line)
{
    if (write_pointer >= data.size()) return;
    line.copy(data.data() + write_pointer, width);
    write_pointer += width;
}
