cmake_minimum_required(VERSION 3.10)

project(Json)

file(GLOB_RECURSE sources *.cpp)
add_library(Json STATIC ${sources})

target_include_directories(Json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Json ARCHIVE DESTINATION lib)
