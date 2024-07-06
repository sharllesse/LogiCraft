cmake_minimum_required(VERSION 3.10)

project(Logi_Editor)

# SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

file(GLOB_RECURSE editor_sources *.h *.cpp)
add_executable(Logi_Editor ${editor_sources})

target_include_directories(Logi_Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Logi_Editor DESTINATION bin)

# Link SFML
target_link_libraries(Logi_Editor PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
include_directories(${SFML_INCLUDE_DIR})

# Link engine
target_link_libraries(Logi_Editor PRIVATE Logi_Engine)

# Link ImGui
target_link_libraries(Logi_Editor PUBLIC ImGui)

if(MSVC)
  target_compile_options(Logi_Editor PUBLIC "/Zi")
  target_link_options(Logi_Editor PUBLIC "/INCREMENTAL")
endif()