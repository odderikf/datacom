cmake_minimum_required(VERSION 3.12)
project(oving6)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
IF(OPENSSL_FOUND)
    SET(OPENSSL_USE_STATIC_LIBS true)
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra -Wno-unused-parameter")

add_executable(oving6 main.cpp)

target_link_libraries(oving6 ${Boost_LIBRARIES})
target_link_libraries(oving6 OpenSSL::Crypto)