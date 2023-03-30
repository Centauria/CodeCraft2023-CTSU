//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"

#include <stdexcept>

Map::Map(size_t height, size_t width)
{
    this->width = width;
    this->height = height;
    data = std::string((width + 2) * (height + 2), '#');
    write_pointer = 1 * (width + 2) + 1;
    rows_written = 0;
    distance_from_barriers = std::make_unique<DMatrix>(height, width);
}
void Map::append_line(const std::string &line)
{
    if (rows_written >= height) return;
    auto s = std::regex_replace(line, active_obj, ".");
    s.copy(data.data() + write_pointer, width);
    write_pointer += (width + 2);
    rows_written++;
}
void Map::refresh_distances()
{
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            (*distance_from_barriers)(j, i) = data[(j + 1) * (width + 2) + i + 1] == '.' ? 127 : 0;
        }
    }
}
char &Map::operator()(size_t y, size_t x)
{
    if (y >= height || x >= width)
        throw std::out_of_range("Index out of range");
    return data[(y + 1) * (width + 2) + x + 1];
}
