cmake_minimum_required(VERSION 3.10)

project(LogiCraft)

# SFML
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

# Install SFML DLLs
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/openal32.dll 							DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/openal32.dll							DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-audio-2.dll 						DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-audio-d-2.dll 						DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-graphics-2.dll 					DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-graphics-d-2.dll 					DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-network-2.dll 						DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-network-d-2.dll 					DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-system-2.dll 						DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-system-d-2.dll 					DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-window-2.dll 						DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES	${CMAKE_CURRENT_SOURCE_DIR}/Code/SFML-2.6.1/bin/sfml-window-d-2.dll 					DESTINATION ${CMAKE_INSTALL_BINDIR})
