cmake_minimum_required(VERSION 3.16)
project(topdown C)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlite3")
find_package(SDL3 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

# Add client target
add_executable(client 
  src/client/client.c)
target_link_libraries(client PRIVATE SDL3::SDL3)

# Add server target
add_executable(server
  src/server/server.c)
target_link_libraries(server PRIVATE Threads::Threads SQLite::SQLite3)
