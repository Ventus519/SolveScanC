cmake_minimum_required(VERSION 4.0)
project(CubeTracker C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
add_subdirectory(Cube3)
include_directories(Cube3)

set(SRCS src_method_trackers/RouxTracker.c
         src_method_trackers/CFOPTracker.c
         src_method_trackers/MoveStack.c
         src_method_trackers/BeginnersTracker.c
         CubeTracker.c
         CubeTracker.h)


add_library(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE Cube3)

