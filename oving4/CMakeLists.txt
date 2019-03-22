cmake_minimum_required(VERSION 3.10)
project(oving4)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

add_executable(mathserver mathserver.cpp)
add_executable(mathclient mathclient.cpp)


include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(mathserver ${Boost_LIBRARIES})
target_link_libraries(mathclient ${Boost_LIBRARIES})