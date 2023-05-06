cmake_minimum_required(VERSION 3.0)
project(chat)

set(CMAE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/db)
include_directories(${PROJECT_SOURCE_DIR}/include/model)
include_directories(${PROJECT_SOURCE_DIR}/include/redis)
add_subdirectory(src)