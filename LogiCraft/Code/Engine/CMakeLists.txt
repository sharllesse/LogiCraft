cmake_minimum_required(VERSION 3.10)

project(Logi_Engine)

# SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

# Logi_Engine
file(GLOB_RECURSE engine_sources *.cpp)
add_library(Logi_Engine SHARED ${engine_sources})

target_compile_definitions(Logi_Engine PRIVATE LOGI_ENGINE_EXPORTS)

target_include_directories(Logi_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Logi_Engine
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Link SFML statically
target_link_libraries(Logi_Engine PUBLIC sfml-system sfml-window sfml-graphics)
