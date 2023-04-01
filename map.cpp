//
// Created by Centauria V. CHEN on 2023/3/30.
//

#include "map.h"

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
    DMatrix result{height + 2, width + 2};
    for (int j = 0; j < height + 2; ++j)
    {
        for (int i = 0; i < width + 2; ++i)
        {
            result(j, i) = data[j * (width + 2) + i] == '.' ? 3 : 0;
        }
    }
    const size_t kernel_size = 3;
    for (int n = 0; n < 2; ++n)
    {
        std::vector<std::tuple<size_t, size_t>> dots;
        for (int j = kernel_size / 2; j < height + 2 - kernel_size / 2; ++j)
        {
            for (int i = kernel_size / 2; i < width + 2 - kernel_size / 2; ++i)
            {
                if (result(j, i) != 3) continue;
                for (int u = -1; u <= 1; ++u)
                {
                    bool placed = false;
                    for (int v = -1; v <= 1; ++v)
                    {
                        if (result(j + u, i + v) == n)
                        {
                            dots.emplace_back(j, i);
                            placed = true;
                            break;
                        }
                    }
                    if (placed) break;
                }
            }
        }
        if (dots.empty()) break;
        for (auto [j, i]: dots)
        {
            result(j, i) = n + 1;
        }
    }
    return result;
}
