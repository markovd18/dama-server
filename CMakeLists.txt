#
# Author: markovd@students.zcu.cz
#

cmake_minimum_required(VERSION 3.15)
project(dama_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find all files in src/*** subdirectories
file(GLOB APP src/app/*.h src/app/*.cpp)
file(GLOB SOCKET src/socket/*.h src/socket/*.cpp)
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