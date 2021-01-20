#
# Author: markovd@students.zcu.cz
#

cmake_minimum_required(VERSION 3.10)
project(dama_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GCC_COMPILE_FLAGS "--std=c++17 -lstdc++fs")
set(CMAKE_CXX_FLAGS ${GCC_COMPILE_FLAGS})

# find all files in src/*** subdirectories
file(GLOB APP src/app/*.h src/app/*.cpp)
file(GLOB SOCKET_REPO src/socket/repository/*.h src/socket/repository/*.cpp)
file(GLOB SOCKET_SERVICE src/socket/service/*.h src/socket/service/*.cpp)
file(GLOB SOCKET_VO src/socket/vo/*.h src/socket/vo/*.cpp)
file(GLOB SOCKET src/socket/*.h src/socket/*.cpp ${SOCKET_REPO} ${SOCKET_SERVICE} ${SOCKET_VO})
file(GLOB UTILS src/utils/*.h src/utils/*.cpp)
file(GLOB EXCEPTION src/exception/*.h src/exception/*.cpp)
file(GLOB REQUEST src/request/*.h src/request/*.cpp)
file(GLOB GAME_VO src/game/vo/*.h src/game/vo/*.cpp)
file(GLOB GAME_SERVICE src/game/service/*.h src/game/service/*.cpp)
file(GLOB GAME_REPO src/game/repository/*.h src/game/repository/*.cpp)
file(GLOB GAME ${GAME_VO} ${GAME_SERVICE} ${GAME_REPO})

# grouping all source files together
file(GLOB SOURCES ${APP} ${SOCKET} ${UTILS} ${EXCEPTION} ${REQUEST} ${GAME})

#setting executable
add_executable(dama_server ${SOURCES})

#setting output directory to root directory
set_target_properties(dama_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})