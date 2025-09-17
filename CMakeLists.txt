cmake_minimum_required(VERSION 3.16)
project(topdown C)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlite3")
find_package(SDL3 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(client 
  src/network.c
  src/channel.c
  src/client.c)
target_link_libraries(client PRIVATE SDL3::SDL3)
target_precompile_headers(client PRIVATE include/pch.h)

add_executable(server
  src/network.c
  src/channel.c
  src/server.c)
target_link_libraries(server PRIVATE Threads::Threads SQLite::SQLite3)
target_precompile_headers(server PRIVATE include/pch.h)
