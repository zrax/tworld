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

if(NOT WIN32)
    set(SHARE_DIR "${CMAKE_INSTALL_PREFIX}/share/tworld" CACHE STRING
        "Directory for shared files")
    add_definitions(-DROOTDIR="${SHARE_DIR}")
    add_definitions(-Dstricmp=strcasecmp)

    file(GLOB TW_SETS "${CMAKE_CURRENT_SOURCE_DIR}/sets/*.dac")
    file(GLOB TW_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data/*.dat")
    file(GLOB TW_RES  "${CMAKE_CURRENT_SOURCE_DIR}/res/rc"
                      "${CMAKE_CURRENT_SOURCE_DIR}/res/*.bmp"
                      "${CMAKE_CURRENT_SOURCE_DIR}/res/*.txt"
                      "${CMAKE_CURRENT_SOURCE_DIR}/res/*.wav")
    file(GLOB TW_MAN6 "${CMAKE_CURRENT_SOURCE_DIR}/docs/*.6")

    install(FILES ${TW_SETS} DESTINATION "${SHARE_DIR}/sets")
    install(FILES ${TW_DATA} DESTINATION "${SHARE_DIR}/data")
    install(FILES ${TW_RES}  DESTINATION "${SHARE_DIR}/res")
    install(FILES ${TW_MAN6} DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man6")
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

if(WIN32)
    install(TARGETS ${TWORLD_EXE} RUNTIME DESTINATION .)
else()
    install(TARGETS ${TWORLD_EXE} RUNTIME DESTINATION bin)
endif()
