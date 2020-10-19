cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(battleships)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# This is the main part:
set(PROJECT_NAME battleships)
file(GLOB PROJECT_SOURCES
    src/main.cpp
	src/*/*.cpp
    src/*/*.hpp
)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib-cpp)
