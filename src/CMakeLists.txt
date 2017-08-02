cmake_minimum_required(VERSION 3.7)
project(TwitterMicroservices)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/include/libbson-1.0)
include_directories(/usr/local/include/libmongoc-1.0)
#include_directories(/usr/local/Cellar/thrift/0.10.0/include/)
find_package(Threads)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_executable (
        ProcessUniqueIDServer
        ProcessUniqueIDServer.cpp
        ../gen-cpp/ProcessUniqueID.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp

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
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
)
target_link_libraries (
        ProcessTextServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ProcessImageServer
        ProcessImageServer.cpp
        ../gen-cpp/ProcessImage.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp

)
target_link_libraries (
        ProcessImageServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ProcessVideoServer
        ProcessVideoServer.cpp
        ../gen-cpp/ProcessVideo.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
)
target_link_libraries (
        ProcessVideoServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ProcessURLServer
        ProcessURLServer.cpp
        ../gen-cpp/ProcessURL.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
)
target_link_libraries (
        ProcessURLServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ProcessTagServer
        ProcessTagServer.cpp
        ../gen-cpp/ProcessTag.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
)
target_link_libraries (
        ProcessTagServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ComposeTweetServer
        ComposeTweetServer.cpp
        ../gen-cpp/ComposeTweet.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoTweetStorage.cpp
        ../gen-cpp/WriteUserGraph.cpp
        ../gen-cpp/WriteTimeline.cpp
        ../gen-cpp/BlockStats.cpp

)
target_link_libraries (
        ComposeTweetServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ComposeReplyServer
        ComposeReplyServer.cpp
        ../gen-cpp/ComposeReply.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoTweetStorage.cpp
        ../gen-cpp/WriteTimeline.cpp
        ../gen-cpp/BlockStats.cpp

)
target_link_libraries (
        ComposeReplyServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ComposeRetweetServer
        ComposeRetweetServer.cpp
        ../gen-cpp/ComposeRetweet.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoTweetStorage.cpp
        ../gen-cpp/WriteUserGraph.cpp
        ../gen-cpp/WriteTimeline.cpp
        ../gen-cpp/BlockStats.cpp

)
target_link_libraries (
        ComposeRetweetServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ComposeDMServer
        ComposeDMServer.cpp
        ../gen-cpp/ComposeDM.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoDMStorage.cpp
        ../gen-cpp/WriteDM.cpp

)
target_link_libraries (
        ComposeDMServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

#add_executable (
#        MemcachedServer
#        MemcachedServer.cpp
#        ../gen-cpp/Memcached.cpp
#
#)
#target_link_libraries (
#        MemcachedServer
#        libthrift.a
#        memcached
#)



add_executable (
        MongoTweetStorageServer
        MongoTweetStorageServer.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoTweetStorage.cpp
        ../gen-cpp/Generator.cpp
        

)

target_link_libraries (
        MongoTweetStorageServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        

)

add_executable (
        MongoDMStorageServer
        MongoDMStorageServer.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
        ../gen-cpp/MongoDMStorage.cpp
        ../gen-cpp/Generator.cpp

)

target_link_libraries (
        MongoDMStorageServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        

)

add_executable (
        WriteUserGraphServer
        WriteUserGraphServer.cpp
        ../gen-cpp/WriteUserGraph.cpp
        ../gen-cpp/Generator.cpp
)

target_link_libraries (
        WriteUserGraphServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        WriteTimelineServer
        WriteTimelineServer.cpp
        ../gen-cpp/WriteTimeline.cpp
        ../gen-cpp/Generator.cpp
)

target_link_libraries (
        WriteTimelineServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        WriteDMServer
        WriteDMServer.cpp
        ../gen-cpp/WriteDM.cpp
        ../gen-cpp/Generator.cpp
)

target_link_libraries (
        WriteDMServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        WriteFavoriteServer
        WriteFavoriteServer.cpp
        ../gen-cpp/WriteFavorite.cpp
        ../gen-cpp/Generator.cpp
)

target_link_libraries (
        WriteFavoriteServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}
        
        
)

add_executable (
        ReadTweetServer
        ReadTweetServer.cpp
        ../gen-cpp/ReadTweet.cpp
        ../gen-cpp/MongoTweetStorage.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp
)

target_link_libraries (
        ReadTweetServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable (
        ReadTimelineServer
        ReadTimelineServer.cpp
        ../gen-cpp/ReadTimeline.cpp
        ../gen-cpp/TwitterMicroservices_types.cpp

)

target_link_libraries (
        ReadTimelineServer
        libthrift.a
        libmongoc-1.0.so
        libbson-1.0.so
        memcached
        ${CMAKE_THREAD_LIBS_INIT}

)

add_executable (
        BlockStatsServer
        BlockStatsServer.cpp
        ../gen-cpp/BlockStats.cpp



)

target_link_libraries (
        BlockStatsServer
        libthrift.a
        ${CMAKE_THREAD_LIBS_INIT}
        
)



