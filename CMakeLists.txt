
cmake_minimum_required(VERSION 3.16)
project(topdown CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
find_package(SDL3 REQUIRED)

# Add client target
add_executable(client src/client/client.cpp)
target_link_libraries(client PRIVATE SDL3::SDL3)

# Add server target
add_executable(server src/server/server.cpp)
target_link_libraries(server PRIVATE pthread)