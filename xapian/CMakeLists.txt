cmake_minimum_required(VERSION 3.7)
project(TwitterMicroservices)

set(CMAKE_CXX_STANDARD 11)

# include_directories(/usr/local/Cellar/xapian/1.4.3/include/xapian)
# include_directories(/usr/local/Cellar/xapian/1.4.3/include/)
find_package(Threads)

link_directories(/usr/local/lib/)
add_executable (
        XapianServer
        XapianServer.cpp
        ../gen-cpp/Xapian.cpp

)
target_link_libraries (
        XapianServer
        libthrift.a
        libthriftnb.a
        libevent.a
        libxapian.so
        ${CMAKE_THREAD_LIBS_INIT}
)