
cmake_minimum_required(VERSION 3.16)
project(topdown CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
find_package(SDL3 REQUIRED)

add_executable(topdown src/main.cpp)
target_include_directories(topdown PRIVATE "/opt/homebrew/opt/sdl3/include/SDL3")
target_link_directories(topdown PRIVATE "/opt/homebrew/opt/sdl3/lib")
target_link_libraries(topdown PRIVATE SDL3::SDL3)