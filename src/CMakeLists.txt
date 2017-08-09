cmake_minimum_required(VERSION 3.8)
project(NetflixMicroservices)

include_directories(../include)


include_directories(/usr/local/include/libbson-1.0)
include_directories(/usr/local/include/libmongoc-1.0)
include_directories("/usr/local/include/nlohmann/")

find_package(Threads)
set(CMAKE_CXX_STANDARD 14)
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
        ${CMAKE_THREAD_LIBS_INIT}
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
        ${CMAKE_THREAD_LIBS_INIT}
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
        ${CMAKE_THREAD_LIBS_INIT}
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
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        ComposeReviewServer
        ComposeReviewServer.cpp
        ../gen-cpp/ComposeReview.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/UserReviewDB.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        ComposeReviewServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        MovieReviewDBServer
        MovieReviewDBServer.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        MovieReviewDBServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable (
        UserReviewDBServer
        UserReviewDBServer.cpp
        ../gen-cpp/UserReviewDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        UserReviewDBServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
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
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable(
        MovieInfoStorageServer
        MovieInfoStorageServer.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        MovieInfoStorageServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable(
        GetPlotServer
        GetPlotServer.cpp
        ../gen-cpp/GetPlot.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetPlotServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetThumbnailServer
        GetThumbnailServer.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/GetThumbnail.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetThumbnailServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetRatingServer
        GetRatingServer.cpp
        ../gen-cpp/GetRating.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetRatingServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetCastInfoServer
        GetCastInfoServer.cpp
        ../gen-cpp/GetCastInfo.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetCastInfoServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable(
        GetPhotoServer
        GetPhotoServer.cpp
        ../gen-cpp/GetPhoto.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetPhotoServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable(
        GetVideoServer
        GetVideoServer.cpp
        ../gen-cpp/GetVideo.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetVideoServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)

add_executable(
        GetMovieReviewServer
        GetMovieReviewServer.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/GetMovieReview.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        GetMovieReviewServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetWatchNextServer
        GetWatchNextServer.cpp
        ../gen-cpp/GetWatchNext.cpp
        ../gen-cpp/ComposePage.cpp
        ../include/netflix_microservices.h
        ../gen-cpp/NetflixMicroservices_types.cpp

)

target_link_libraries (
        GetWatchNextServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        ComposePageServer
        ComposePageServer.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/GetCastInfo.cpp
        ../gen-cpp/GetMovieReview.cpp
        ../gen-cpp/GetPhoto.cpp
        ../gen-cpp/GetPlot.cpp
        ../gen-cpp/GetVideo.cpp
        ../gen-cpp/GetRating.cpp
        ../gen-cpp/GetThumbnail.cpp
        ../gen-cpp/GetWatchNext.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp
        ../include/netflix_microservices.h

)

target_link_libraries (
        ComposePageServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)
