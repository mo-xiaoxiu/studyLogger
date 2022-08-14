cmake_minimum_required(VERSION 3.10)
project(test_log)

set(CMAKE_CXX_FLAGS "-std=c++17 -g")
set(SRC_FILE main.cpp logger.cpp)


add_executable(${PROJECT_NAME} ${SRC_FILE})

