
cmake_minimum_required(VERSION 3.16)
project(topdown CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
find_package(SDL3 REQUIRED)

# Add client target
add_executable(client src/client/client.cpp)
target_include_directories(client PRIVATE "/opt/homebrew/opt/sdl3/include/SDL3")
target_link_directories(client PRIVATE "/opt/homebrew/opt/sdl3/lib")
target_link_libraries(client PRIVATE SDL3::SDL3)

# Add server target
add_executable(server src/server/server.cpp)