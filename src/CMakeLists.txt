cmake_minimum_required(VERSION 3.5)
project(NetflixMicroservices)

include_directories(../include
        /usr/local/include/libbson-1.0
        /usr/local/include/libmongoc-1.0
        /usr/local/include/nlohmann)


find_package(Threads)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



add_executable (
        ProcessUniqueIDServer
        process_unique_id.cpp
        ../gen-cpp/ProcessUniqueID.cpp
        ../gen-cpp/ComposeReview.cpp
        utils.cpp utils.h)

target_link_libraries (
        ProcessUniqueIDServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        ProcessTextServer
        process_text.cpp
        ../gen-cpp/ProcessText.cpp
        ../gen-cpp/ComposeReview.cpp
        utils.cpp utils.h)

target_link_libraries (
        ProcessTextServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        AssignRatingServer
        process_rating.cpp
        ../gen-cpp/AssignRating.cpp
        ../gen-cpp/ComposeReview.cpp
        utils.cpp utils.h)

target_link_libraries (
        AssignRatingServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        ProcessMovieIDServer
        process_movie_id.cpp
        ../gen-cpp/ProcessMovieID.cpp
        ../gen-cpp/ComposeReview.cpp
        utils.cpp utils.h)

target_link_libraries (
        ProcessMovieIDServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        ComposeReviewServer
        compose_review.cpp
        ../gen-cpp/ComposeReview.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/UserReviewDB.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        ComposeReviewServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
        MovieReviewDBServer
        movie_db.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        user_db.cpp
        ../gen-cpp/UserReviewDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        review_store.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        movie_info_store.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        get_plot.cpp
        ../gen-cpp/GetPlot.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        GetPlotServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetThumbnailServer
        get_thumbnail.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/GetThumbnail.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        GetThumbnailServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetRatingServer
        get_rating.cpp
        ../gen-cpp/GetRating.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        GetRatingServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetCastInfoServer
        get_cast_info.cpp
        ../gen-cpp/GetCastInfo.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/MovieInfoStorage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        get_photo.cpp
        ../gen-cpp/GetPhoto.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        get_video.cpp
        ../gen-cpp/GetVideo.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

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
        get_movie_review.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/GetMovieReview.cpp
        ../gen-cpp/ReviewStorage.cpp
        ../gen-cpp/MovieReviewDB.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        GetMovieReviewServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        GetWatchNextServer
        get_watch_next.cpp
        ../gen-cpp/GetWatchNext.cpp
        ../gen-cpp/ComposePage.cpp
        ../gen-cpp/NetflixMicroservices_types.cpp

        utils.cpp utils.h)

target_link_libraries (
        GetWatchNextServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        ComposePageServer
        compose_page.cpp
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

        utils.cpp utils.h)

target_link_libraries (
        ComposePageServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable(
        UserAccountServer
        user_account.cpp
        ../gen-cpp/UserAccount.cpp

        utils.cpp utils.h)

target_link_libraries (
        UserAccountServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        libmongoc-1.0.so
        libbson-1.0.so
        memcached

)
