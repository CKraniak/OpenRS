# Copyright (C) 2016, Chesley Kraniak
#
# This code is distributed under the terms of the GPLv3 license, which should
# have been included with this file in the root directory as "LICENSE.txt". If
# you do not have a copy of the license, see:
#     http://www.gnu.org/licenses/gpl-3.0.txt
# or:
#     https://github.com/CKraniak/OpenRS
# for the license.

# **************************************************************************
#
#     BASIC SETUP

# Set the final executable's name
project(openrs_v0.9.0-prealpha-4)

# Require CMake 2.8. I don't know if this is needed, but since I use it, I
# don't know how far back versions can go before breaking.
cmake_minimum_required(VERSION 2.8)

# Find all of the .h and .cpp files in the project directory, and list their
# names in SRC_LISTS.
file(GLOB_RECURSE SRC_LIST . *.h *.cpp)

# **************************************************************************
#
#     BOOST

# Find the Boost library's location. This command requires Boost be in your
# system's path; if it isn't, you can manually set the search path (before the
# find_package command) using:
#     SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/path/to/boost")
# and / or:
#     SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/path/to/boost/lib")
# Note: I'm pretty sure I'm not using any Boost libraries ... yet.
find_package(Boost 1.59.0 REQUIRED)
if(Boost_FOUND)
    # If we have boost, set our include directories to include Boost headers
    include_directories(${Boost_INCLUDE_DIRS})
    # Create a new .exe target. The WIN32 is to stop consoles from popping up.
    add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
    # Link against any Boost libraries, and also OpenGL (important!)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} opengl32)
endif()

# Note that if Boost is not found, the .exe target is not created, so CMake
# doesn't try to compile anything.

# **************************************************************************
#
#     COMPILER AND LINKER FLAGS

# Set g++ compiler flags.

# Set the DEBUG symbol. This causes the ERR_MSGOUT_DEBUG macro to run in all
# source files, and not just main.
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D DEBUG")

# Set the gdb debugging symbols flag. This makes the Qt debugger actually work.
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

# Set our C++ version to C++11.
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set linker flag -static, which I'm pretty sure tries to link in as many
# static libraries as it can if given the choice. This is so the
# "abcd.dll is missing" errors go away.
SET(GCC_COVERAGE_LINK_FLAGS "-static")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
