cmake_minimum_required(VERSION 3.10)
project(oving1)

set(CMAKE_CXX_STANDARD 17)

add_executable(oving1 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(oving1 ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(oving1 PRIVATE -pthread -Wall -Wextra -pedantic -Werror -Wno-reorder -Wno-unused)