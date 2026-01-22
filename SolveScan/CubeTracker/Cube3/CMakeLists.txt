cmake_minimum_required(VERSION 4.0)
project(Cube3 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(SRCS src/Cubie.c
        src/Cube.c)


add_library(${PROJECT_NAME} ${SRCS})

