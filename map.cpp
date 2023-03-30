//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"

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
}
