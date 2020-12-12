cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(
	battleships
	DESCRIPTION "Multiplayer battleships game"
	HOMEPAGE_URL "github.com/jaenek/battleships"
	LANGUAGES C CXX
)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(project_warnings INTERFACE)
if(NOT MSVC)
	include(cmake/Warnings.cmake)
	set_project_warnings(project_warnings)
endif()

find_package(raylib 3.0.0 QUIET)
if (NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG bd512764ff7c82fa6e1395c51accb3ba18162827
	)
	FetchContent_GetProperties(raylib)
	if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(raylib)
		set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
		set(BUILD_TESTING  OFF CACHE BOOL "" FORCE)
		add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
	endif()
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
		add_subdirectory(${raylib-cpp_SOURCE_DIR} ${raylib-cpp_BINARY_DIR})
	endif()
endif()

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
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)

# This is the main part:
file(GLOB PROJECT_SOURCES
	src/main.cpp
	src/board/*
	src/game/*
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE project_options project_warnings raylib raylib-cpp asio)
