project(rtsp_proxy)

cmake_minimum_required(VERSION 3.1.3)

add_subdirectory(3rdparty/ZLMediaKit)

include_directories(3rdparty/ZLMediaKit/src/)
include_directories(3rdparty/ZLMediaKit/3rdpart/ZLToolKit/src/)

file(GLOB MediaServer_src_list ./*.cpp ./*.h)
add_executable(rtsp_proxy ${MediaServer_src_list})

target_link_libraries(rtsp_proxy zlmediakit zltoolkit pthread)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

install(FILES "scripts/rtsp_proxy.service" DESTINATION /etc/systemd/system/)
install(FILES "scripts/config.ini" DESTINATION ${CMAKE_INSTALL_PREFIX}/etc/rtsp_proxy/)
