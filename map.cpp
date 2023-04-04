//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"
#include "path.h"

#include <stdexcept>

GameMap::GameMap(size_t height, size_t width)
{
    this->width = width;
    this->height = height;
    data = std::string((width + 2) * (height + 2), '#');
    write_pointer = 1 * (width + 2) + 1;
    rows_written = 0;
}
void GameMap::append_line(const std::string &line)
{
    if (rows_written >= height) return;
    auto s = std::regex_replace(line, active_obj, ".");
    s.copy(data.data() + write_pointer, width);
    write_pointer += (width + 2);
    rows_written++;
}
char &GameMap::operator()(size_t y, size_t x)
{
    if (y >= height || x >= width)
        throw std::out_of_range("Index out of range");
    return data[(y + 1) * (width + 2) + x + 1];
}
DMatrix GameMap::get_distances()
{
    if (distance_mat) return *distance_mat;
    auto result = std::make_shared<DMatrix>(height + 2, width + 2);
    for (int j = 0; j < height + 2; ++j)
    {
        for (int i = 0; i < width + 2; ++i)
        {
            result->operator()(j, i) = data[j * (width + 2) + i] == '.' ? 3 : 0;
        }
    }
    auto valid_part = DView{*result, {1, 1}, {101, 101}};
    for (int n = 0; n < 2; ++n)
    {
        auto p = convolve(*result, {3, 3}, [n](DView &v) {
            auto center = v(1, 1);
            if (center != 3) return center;
            for (int y = 0; y < 3; ++y)
            {
                for (int x = 0; x < 3; ++x)
                {
                    if (v(y, x) == n) return n + 1.0;
                }
            }
            return center;
        });
        valid_part = p;
    }
    distance_mat = result;
    return *result;
}
bool GameMap::blocked(const Path &path)
{
    return std::any_of(path.cbegin(), path.cend(), [this](const auto &p) {
        return get_distances()(p.y + 1, p.x + 1) <= 1;
    });
}
