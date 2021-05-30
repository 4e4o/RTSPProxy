project(RTSPProxy)

cmake_minimum_required(VERSION 3.1.3)

add_subdirectory(3rdparty/ZLMediaKit)

include_directories(3rdparty/ZLMediaKit/src/)
include_directories(3rdparty/ZLMediaKit/3rdpart/ZLToolKit/src/)

file(GLOB MediaServer_src_list ./*.cpp ./*.h)
add_executable(RTSPProxy ${MediaServer_src_list})

target_link_libraries(RTSPProxy zlmediakit zltoolkit pthread)



