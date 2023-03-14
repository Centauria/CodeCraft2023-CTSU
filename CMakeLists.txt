project(CodeCraftSDK)
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if (!WIN32)
    link_libraries(pthread rt m)
endif (!WIN32)

AUX_SOURCE_DIRECTORY(. src)
ADD_EXECUTABLE(main ${src} src/center.cpp src/center.h src/robot.cpp src/robot.h src/math/point.cpp src/math/Vector2D.h src/math/Vector2D.cpp src/workbench.cpp src/math/pid.cpp src/math/pid.h src/math/function.cpp src/math/function.h src/log/logging.h src/util/string.h)