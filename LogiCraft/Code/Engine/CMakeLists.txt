cmake_minimum_required(VERSION 3.10)

project(Logi_Engine)

# Logi_Engine
file(GLOB_RECURSE engine_sources *.h *.cpp)
add_library(Logi_Engine SHARED ${engine_sources})

target_compile_definitions(Logi_Engine PRIVATE LOGI_ENGINE_EXPORTS)

target_include_directories(Logi_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)

install(TARGETS Logi_Engine RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Link SFML
target_link_libraries(Logi_Engine PUBLIC sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# Link ImGui
target_link_libraries(Logi_Engine PUBLIC ImGui-SFML)

# Link Json
target_link_libraries(Logi_Engine PUBLIC Json)