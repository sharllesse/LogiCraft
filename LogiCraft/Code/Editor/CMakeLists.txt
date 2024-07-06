cmake_minimum_required(VERSION 3.10)

project(Logi_Editor)

file(GLOB_RECURSE editor_sources *.h *.cpp)
add_executable(Logi_Editor ${editor_sources})

target_include_directories(Logi_Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Logi_Editor DESTINATION ${CMAKE_INSTALL_BINDIR})

# Link SFML
target_link_libraries(Logi_Editor PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# Link engine
target_link_libraries(Logi_Editor PRIVATE Logi_Engine)

# Link ImGui
target_link_libraries(Logi_Editor PUBLIC ImGui-SFML)

if(MSVC)
  target_compile_options(Logi_Editor PUBLIC "/Zi")
  target_link_options(Logi_Editor PUBLIC "/INCREMENTAL")
endif()