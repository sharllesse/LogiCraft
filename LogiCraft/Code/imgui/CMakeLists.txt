cmake_minimum_required(VERSION 3.10)

project(ImGui)

file(GLOB_RECURSE sources *.cpp)
add_library(ImGui STATIC ${sources})

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.1/include/)

# Link SFML
target_link_libraries(ImGui PUBLIC sfml-graphics)

install(TARGETS ImGui ARCHIVE DESTINATION lib)
