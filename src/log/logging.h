//
// Created by Centauria V. CHEN on 2023/3/14.
//

#ifndef CODECRAFTSDK_LOGGING_H
#define CODECRAFTSDK_LOGGING_H

#define LOGGING

#ifdef LOGGING

#warning You have enabled LOGGING macro, make sure all files used this macro have "_logging_name" property readable in-place.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

#define LOG(filename, content)                                 \
    {                                                          \
        auto parent_path = fs::path((filename)).parent_path(); \
        fs::create_directories(parent_path);                   \
        std::ofstream ofs((filename), std::ios_base::app);     \
        ofs << _logging_name << ":" << (content) << std::endl; \
        ofs.close();                                           \
    }

#else

#define LOG(x, y)

#endif//LOGGING

#endif//CODECRAFTSDK_LOGGING_H
