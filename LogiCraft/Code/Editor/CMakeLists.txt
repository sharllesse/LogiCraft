cmake_minimum_required(VERSION 3.10)

project(Logi_Editor)

file(GLOB_RECURSE editor_sources *.cpp)
add_executable (Logi_Editor ${editor_sources})

target_include_directories(Logi_Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS Logi_Editor DESTINATION bin)

target_link_libraries(Logi_Editor PRIVATE Logi_Engine)

if(MSVC)
  target_compile_options(Logi_Editor PUBLIC "/ZI")
  target_link_options(Logi_Editor PUBLIC "/INCREMENTAL")
endif()