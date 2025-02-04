cmake_minimum_required(VERSION 3.5.0)
project(infoteck VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

add_executable(Main main.cpp
                src/pcapReader/pcapreader.cpp
                src/Dist/dist.cpp
                src/Handlers/handler.cpp)

target_link_libraries(Main PRIVATE pcap Threads::Threads)

include(CTest)
enable_testing()

