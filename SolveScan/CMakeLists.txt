
cmake_minimum_required(VERSION 4.0)
project(SolveScan C)

set(CMAKE_C_STANDARD 11)


add_subdirectory(Cube3)
include_directories(Cube3)

find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GTK_INCLUDE_DIRS})


message("GTK LIB DIRS: ${GTK_LIBRARY_DIRS}")
include_directories(${GTK_LIBRARY_DIRS})
message("GTK INCLUDE_DIRS: ${GTK_INCLUDE_DIRS}")
message("INCLUDED LIBRARIES: ${GTK_LIBRARIES}" )
ADD_DEFINITIONS(${GTK4_CFLAGS_OTHER})


set(CubeTrackerSRCS CubeTracker/RouxTracker.c
                    CubeTracker/RouxTracker.h
                    CubeTracker/MoveStack.c
                    CubeTracker/MoveStack.h
                    CubeTracker/CubeTracker.c
                    CubeTracker/CubeTracker.h)

add_library(RouxTracker SHARED ${CubeTrackerSRCS})

target_link_libraries(RouxTracker PRIVATE Cube3)

set(APP_SRCS Application/App.c
             Application/App.h)

add_executable(${PROJECT_NAME} main.c ${APP_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES} RouxTracker)

target_compile_options(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS_OTHER})