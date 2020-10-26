cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
set(PROJECT_NAME battleships)
project(${PROJECT_NAME} CXX)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

add_library(project_warnings INTERFACE)
include(cmake/Warnings.cmake)
set_project_warnings(project_warnings)

# raylib
find_package(raylib 3.0.0 QUIET)
if (NOT raylib_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG fa357b8d5d934f54dd1b3cd343b05f06b732d28f
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

# raylib-cpp
find_package(raylib-cpp 3.0.1 QUIET)
if (NOT raylib-cpp_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		raylib-cpp
		GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
		GIT_TAG e657fcb41ff71aa8c5b1ae047d37c1eeb4cff7b7
	)
	FetchContent_GetProperties(raylib-cpp)
	if (NOT raylib-cpp_POPULATED) # Have we downloaded raylib yet?
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(raylib-cpp)
		set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(BUILD_TESTING             OFF CACHE BOOL "" FORCE)
		add_subdirectory(${raylib-cpp_SOURCE_DIR} ${raylib-cpp_BINARY_DIR})
	endif()
endif()

# asio
find_package(asio 1.18.0 QUIET)
if (NOT asio_FOUND)
	include(FetchContent)
	FetchContent_Declare(
		asio
		GIT_REPOSITORY https://github.com/chriskohlhoff/asio
		GIT_TAG be7badc31abcc395cf868de6a1e240c2350bdbf2
	)
	FetchContent_GetProperties(asio)
	if (NOT asio_POPULATED) # Have we downloaded raylib yet?
		set(FETCHCONTENT_QUIET NO)
		FetchContent_Populate(asio)
	endif()
endif()

# This is the main part:
file(GLOB PROJECT_SOURCES
    src/main.cpp
	src/*/*.cpp
    src/*/*.hpp
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
set(raylib_VERBOSE 1)
include_directories(SYSTEM ${raylib_SOURCE_DIR}/src)
include_directories(SYSTEM ${raylib-cpp_SOURCE_DIR}/include)
include_directories(SYSTEM ${asio_SOURCE_DIR}/asio/include)
target_link_libraries(${PROJECT_NAME} PUBLIC project_options project_warnings raylib raylib-cpp)
