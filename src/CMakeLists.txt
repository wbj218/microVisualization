cmake_minimum_required(VERSION 3.8)
project(NetflixMicroservices)

include_directories(../include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable (
        ProcessUniqueIDServer
        ProcessUniqueIDServer.cpp
        ../gen-cpp/ProcessUniqueID.cpp
        ../include/netflix_microservices.h
#        ../gen-cpp/ComposeReview.cpp
)

target_link_libraries (
        ProcessUniqueIDServer
        libthrift.a
)

add_executable (
        ProcessTextServer
        ProcessTextServer.cpp
        ../gen-cpp/ProcessText.cpp
        ../include/netflix_microservices.h
        #        ../gen-cpp/ComposeReview.cpp
)

target_link_libraries (
        ProcessUniqueIDServer
        libthrift.a
)

