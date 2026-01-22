
cmake_minimum_required(VERSION 4.0)
project(SolveScan C)

set(CMAKE_C_STANDARD 11)


add_subdirectory(CubeTracker)
include_directories(CubeTracker)

find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk4)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GTK_INCLUDE_DIRS})


message("GTK LIB DIRS: ${GTK_LIBRARY_DIRS}")
include_directories(${GTK_LIBRARY_DIRS})
message("GTK INCLUDE_DIRS: ${GTK_INCLUDE_DIRS}")
message("INCLUDED LIBRARIES: ${GTK_LIBRARIES}" )
ADD_DEFINITIONS(${GTK4_CFLAGS_OTHER})


set(APP_SRCS Application/App.c
             Application/App.h)

add_executable(${PROJECT_NAME} main.c ${APP_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTK_LIBRARIES} CubeTracker)

target_compile_options(${PROJECT_NAME} PRIVATE ${GTK_CFLAGS_OTHER})