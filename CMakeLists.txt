cmake_minimum_required(VERSION 3.17)
project(dama_server)

set(CMAKE_CXX_STANDARD 17)

# setting sources directory
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

# find all files in src/*** subdirectories
file(GLOB APP ${SOURCES_DIR}/app/*.h ${SOURCES_DIR}/app/*.cpp)

file(GLOB SOCKET ${SOURCES_DIR}/socket/*.h ${SOURCES_DIR}/socket/*.cpp)

# grouping all source files together
file(GLOB SOURCES ${APP} ${SOCKET})

#setting executable
add_executable(dama_server src/app/main.cpp)

#setting output directory to root directory
set_target_properties(dama_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})