cmake_minimum_required(VERSION 3.10)

project(LogiCraft)

# SFML
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

add_subdirectory(Code/imgui/)
add_subdirectory(Code/Engine/)
add_subdirectory(Code/Editor/)
add_subdirectory(Code/json/)

# Set the default startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Logi_Editor)