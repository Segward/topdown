cmake_minimum_required(VERSION 3.16)
project(topdown CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl3")
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sqlite3")
find_package(SDL3 REQUIRED)
find_package(SQLite3 REQUIRED)

# Add client target
add_executable(client 
    src/client/client.cpp 
    src/client/network/network.cpp 
    src/shared/model/player.cpp)
target_link_libraries(client PRIVATE SDL3::SDL3)

# Add server target
add_executable(server 
    src/server/server.cpp
    src/shared/model/player.cpp
    src/server/database/database.cpp
    src/server/dao/playerdao.cpp
    src/server/network/network.cpp)
target_link_libraries(server PRIVATE pthread SQLite3)