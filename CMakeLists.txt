cmake_minimum_required(VERSION 3.17)
project(dama_server)

set(CMAKE_CXX_STANDARD 17)

# setting sources directory
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

# find all files in src/*** subdirectories
file(GLOB APP ${SOURCES_DIR}/app/*.h ${SOURCES_DIR}/app/*.cpp)
file(GLOB SOCKET ${SOURCES_DIR}/socket/*.h ${SOURCES_DIR}/socket/*.cpp)
file(GLOB UTILS ${SOURCES_DIR}/utils/*.h ${SOURCS_DIR}/utils/*.cpp)

add_library(SERVER ${SOURCES_DIR}/app/Server.cpp)
add_library(SOCKET_LISTENER ${SOURCES_DIR}/socket/SocketListener.cpp)
add_library(PARAM_UTILS ${SOURCES_DIR}/utils/ParameterUtils.cpp)

# grouping all source files together
file(GLOB SOURCES ${APP} ${SOCKET} ${UTILS})

#setting executable
add_executable(dama_server src/app/main.cpp)

target_link_libraries(dama_server SERVER SOCKET_LISTENER PARAM_UTILS)
#setting output directory to root directory
set_target_properties(dama_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})