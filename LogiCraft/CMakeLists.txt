cmake_minimum_required(VERSION 3.10)

project(LogiCraft)

# SFML
message(STATUS "CMAKE_INSTALL_BINDIR: ${CMAKE_INSTALL_BINDIR}")
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6.1 COMPONENTS system window graphics network audio REQUIRED)

#ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Code/imgui")

add_subdirectory(Code/imgui-sfml/)
add_subdirectory(Code/Engine/)
add_subdirectory(Code/Editor/)
add_subdirectory(Code/json/)

# Set the default startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Logi_Editor)