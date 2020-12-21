cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(
	battleships
	DESCRIPTION "Multiplayer battleships game"
	HOMEPAGE_URL "github.com/jaenek/battleships"
)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(project_warnings INTERFACE)
if(NOT MSVC)
	include(cmake/Warnings.cmake)
	set_project_warnings(project_warnings)
endif()

find_package(raylib-cpp 3.0.1 QUIET)
if (NOT raylib-cpp_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib-cpp
		GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
		GIT_TAG ebb379029bd8f73dc42791f5d8fc9c9e607ffd1a
	)
	FetchContent_GetProperties(raylib-cpp)
	if (NOT raylib-cpp_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(raylib-cpp)
		set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(BUILD_TESTING             OFF CACHE BOOL "" FORCE)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
		add_subdirectory(${raylib-cpp_SOURCE_DIR}/vendor/raylib)
	endif()
endif()
add_library(raylib-cpp INTERFACE)
target_compile_options(raylib-cpp INTERFACE -DGRAPHICS="GRAPHICS_API_OPENGL_11")
target_link_libraries(raylib-cpp INTERFACE raylib)
target_include_directories(raylib-cpp SYSTEM INTERFACE ${raylib-cpp_SOURCE_DIR}/include)

find_package(asio 1.18.0 QUIET)
if (NOT asio_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		asio
		GIT_REPOSITORY https://github.com/chriskohlhoff/asio
		GIT_TAG be7badc31abcc395cf868de6a1e240c2350bdbf2
	)
	FetchContent_GetProperties(asio)
	if (NOT asio_POPULATED)
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(asio)
	endif()
endif()
add_library(asio INTERFACE)
target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)

# This is the main part:
file(GLOB PROJECT_SOURCES
	src/main.cpp
	src/board/*
	src/game/*
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE project_options project_warnings raylib-cpp asio)
