# CMake build system for Tile World 2
#
# Copyright (C) 2020 by Michael Hansen, under the GNU General Public
# License. No warranty. See COPYING for details.

cmake_minimum_required(VERSION 3.1)
project(tworld)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(OSHW "qt" CACHE STRING "OS/HW Flavor to build (sdl or qt)")
set_property(CACHE OSHW PROPERTY STRINGS qt sdl)

if(OSHW STREQUAL "qt")
    set(TWORLD_EXE "tworld2")
elseif(OSHW STREQUAL "sdl")
    set(TWORLD_EXE "tworld")
else()
    message(FATAL_ERROR "${OSHW} is not a valid OSHW setting")
endif()

# We still require SDL even for the Qt build...
find_package(SDL REQUIRED)
if(OSHW STREQUAL "qt")
    set(CMAKE_AUTOUIC TRUE)
    set(CMAKE_AUTOMOC TRUE)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Xml)
    add_definitions(-DTWPLUSPLUS)
endif()

add_subdirectory(oshw-${OSHW})

add_executable(${TWORLD_EXE} WIN32 tworld.c)
target_sources(${TWORLD_EXE} PRIVATE
    cmdline.h
    cmdline.c
    comptime.h
    defs.h
    encoding.h
    encoding.c
    err.h
    err.c
    fileio.h
    fileio.c
    gen.h
    help.h
    help.c
    logic.h
    lxlogic.c
    messages.h
    messages.cpp
    mslogic.c
    oshw.h
    play.h
    play.c
    random.h
    random.c
    res.h
    res.c
    score.h
    score.cpp
    series.h
    series.c
    settings.h
    settings.cpp
    solution.h
    solution.c
    state.h
    unslist.h
    unslist.c
    ver.h
)

target_link_libraries(${TWORLD_EXE} PRIVATE oshw-${OSHW})

# Generate the current compile time
if(WIN32)
    # TODO: It would be nice to use the format below on Win32 as well...
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/comptime.bat"
         "echo #define COMPILE_TIME \"%DATE% %TIME%\" > comptime.h")
    add_custom_command(
        OUTPUT comptime.h
        COMMAND comptime.bat
    )
else()
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/comptime.sh"
         "echo \"#define COMPILE_TIME \\\"`date '+%Y %b %e %T %Z'`\\\"\" > comptime.h")
    add_custom_command(
        OUTPUT comptime.h
        COMMAND /bin/sh comptime.sh
    )
endif()

set_property(SOURCE comptime.h PROPERTY SKIP_AUTOGEN ON)
target_include_directories(${TWORLD_EXE} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
