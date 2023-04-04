//
// Created by Centauria V. CHEN on 2023/3/30.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include "matrix.h"
#include "path.h"
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <regex>
#include <string>

class GameMap : public AbstractMatrix<char>
{
public:
    GameMap(size_t height, size_t width);
    GameMap(const GameMap &map) = default;
    void append_line(const std::string &line);
    DMatrix get_distances();
    bool blocked(const Path &path);

    char &operator()(size_t y, size_t x) override;

private:
    std::string data;
    size_t width;
    size_t height;

    size_t write_pointer = 0;
    size_t rows_written = 0;
    std::shared_ptr<DMatrix> distance_mat = nullptr;

    const std::regex active_obj{R"([^.#])"};
};

#endif//CODECRAFTSDK_MAP_H
