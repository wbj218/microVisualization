cmake_minimum_required(VERSION 3.7)
project(TwitterMicroservices)
include_directories("/usr/local/include/nlohmann/")
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(./src)

add_subdirectory(./user_graph)

add_subdirectory(./xapian)
