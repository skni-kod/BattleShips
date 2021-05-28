cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(
	battleships
	DESCRIPTION "Multiplayer battleships game"
	HOMEPAGE_URL "github.com/skni-kod/battleships"
	LANGUAGES CXX
)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
include(cmake/Warnings.cmake)
set_project_warnings(project_warnings)

include(cmake/Doxygen.cmake)
enable_doxygen(src/)

include(FetchContent)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
	FetchContent_Declare(
		raylib
		URL https://github.com/raysan5/raylib/archive/master.tar.gz
		GIT_TAG 3.7.0
	)

	FetchContent_GetProperties(raylib)
	if (NOT raylib_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(OPENGL_VERSION "3.3" CACHE BOOL "" FORCE)
		add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
	endif()
endif()

find_package(asio QUIET)
if (NOT asio_FOUND)
	FetchContent_Declare(
		asio
		GIT_REPOSITORY https://github.com/chriskohlhoff/asio
		GIT_TAG asio-1-18-0
	)
	FetchContent_GetProperties(asio)
	if (NOT asio_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(asio)
	endif()
endif()
add_library(asio INTERFACE)

find_package(Threads)
target_link_libraries(asio INTERFACE Threads::Threads)

if(MSVC)
	target_compile_options(asio INTERFACE -DASIO_STANDALONE /experimental:external /external:W0 /external:I ${asio_SOURCE_DIR}/asio/include)
else()
	target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)
endif()

# This is the main part:
file(GLOB PROJECT_SOURCES
	src/ui/button.cpp
	src/ui/selection.cpp
	src/game/game_ships.cpp
	src/game/game_board.cpp
	src/game/game_net_interface.cpp
	src/main_window/main_window.cpp
	src/main.cpp
)
add_executable(${PROJECT_NAME}_client ${PROJECT_SOURCES})
target_compile_definitions(${PROJECT_NAME}_client PRIVATE CLIENT)
target_include_directories(${PROJECT_NAME}_client PRIVATE ${raylib_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}_client PRIVATE project_options project_warnings raylib asio)

add_executable(${PROJECT_NAME}_server src/main.cpp)
target_compile_definitions(${PROJECT_NAME}_server PRIVATE SERVER)
target_link_libraries(${PROJECT_NAME}_server PRIVATE project_options project_warnings asio)

add_executable(${PROJECT_NAME}_client_example src/main.cpp)
target_compile_definitions(${PROJECT_NAME}_client_example PRIVATE CLIENT EXAMPLE)
target_link_libraries(${PROJECT_NAME}_client_example PRIVATE project_options project_warnings raylib asio)

add_executable(${PROJECT_NAME}_server_example src/main.cpp)
target_compile_definitions(${PROJECT_NAME}_server_example PRIVATE SERVER EXAMPLE)
target_link_libraries(${PROJECT_NAME}_server_example PRIVATE project_options project_warnings asio)
