cmake_minimum_required(VERSION 3.8)
project(NetflixMicroservices)

include_directories(../include)


include_directories(/usr/local/include/libbson-1.0)
include_directories(/usr/local/include/libmongoc-1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable (
        ProcessUniqueIDServer
        ProcessUniqueIDServer.cpp
        ../gen-cpp/ProcessUniqueID.cpp
        ../include/netflix_microservices.h
        ../gen-cpp/ComposeReview.cpp
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
        ../gen-cpp/ComposeReview.cpp
)

target_link_libraries (
        ProcessTextServer
        libthrift.a
)

add_executable (
        AssignRatingServer
        AssignRatingServer.cpp
        ../gen-cpp/AssignRating.cpp
        ../include/netflix_microservices.h
        ../gen-cpp/ComposeReview.cpp
)

target_link_libraries (
        AssignRatingServer
        libthrift.a
)

add_executable (
        ProcessMovieIDServer
        ProcessMovieIDServer.cpp
        ../gen-cpp/ProcessMovieID.cpp
        ../include/netflix_microservices.h
        ../gen-cpp/ComposeReview.cpp
)

target_link_libraries (
        ProcessMovieIDServer
        libthrift.a
)

add_executable (
        ComposeReviewServer
        ComposeReviewServer.cpp
        ../gen-cpp/ComposeReview.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        ComposeReviewServer
        libthrift.a
)

add_executable (
        WriteMovieDBServer
        WriteMovieReviewDBServer.cpp
        ../gen-cpp/WriteMovieDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        WriteMovieDBServer
        libthrift.a
        libmongoc-1.0.a
        libbson-1.0.a
        memcached

)

add_executable (
        WriteUserDBServer
        WriteUserReviewDBServer.cpp
        ../gen-cpp/WriteUserDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        WriteUserDBServer
        libthrift.a
        libmongoc-1.0.a
        libbson-1.0.a
        memcached

)

add_executable (
        ReviewStorageServer
        ReviewStorageServer.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        ReviewStorageServer
        libthrift.a
        libmongoc-1.0.a
        libbson-1.0.a
        memcached

)
