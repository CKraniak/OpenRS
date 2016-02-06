project(openrs_v0.9.0.2)
cmake_minimum_required(VERSION 2.8)
file(GLOB_RECURSE SRC_LIST . *.h *.cpp)

find_package(Boost 1.59.0 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} opengl32)
endif()

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D DEBUG")
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(GCC_COVERAGE_LINK_FLAGS "-static")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
