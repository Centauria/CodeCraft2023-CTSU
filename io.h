//
// Created by Centauria V. CHEN on 2023/4/7.
//

#ifndef CODECRAFTSDK_IO_H
#define CODECRAFTSDK_IO_H

#include "matrix.h"

std::vector<std::string> input_map();
std::vector<std::string> input_frame();
CMatrix read_map(const std::vector<std::string> &map);

#endif//CODECRAFTSDK_IO_H
