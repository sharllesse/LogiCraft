cmake_minimum_required(VERSION 3.10)

project(ImGui)

# SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS graphics REQUIRED)

file(GLOB_RECURSE sources *.cpp)
add_library(ImGui STATIC ${sources})

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.1/include/)

# Link SFML statically
target_link_libraries(ImGui PUBLIC sfml-graphics)

install(TARGETS ImGui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

