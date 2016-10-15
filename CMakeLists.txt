cmake_minimum_required(VERSION 3.6)
project(polyphase_sorting_by_miczi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Cone.cpp Cone.h)
add_executable(polyphase_sorting_by_miczi ${SOURCE_FILES})