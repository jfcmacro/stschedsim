cmake_minimum_required(VERSION 3.25)
project(stschedsim)
include_directories(
   ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
   ${PROJECT_SOURCE_DIR}/include/*.hpp
   ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(stschedsim ${all_SRCS})
