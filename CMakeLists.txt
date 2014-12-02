cmake_minimum_required(VERSION 2.8.4)
project(ProjectProgPara)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(ProjectProgPara ${SOURCE_FILES})
if(WIN32)
  target_link_libraries(ProjectProgPara wsock32 ws2_32)
endif()