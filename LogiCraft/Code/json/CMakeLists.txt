cmake_minimum_required(VERSION 3.10)

project(Json)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.hpp)
add_library(Json STATIC ${sources} ${headers})

target_include_directories(Json PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Json
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

