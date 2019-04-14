cmake_minimum_required(VERSION 3.10)
project(oving5)
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(ODB REQUIRED COMPONENTS mysql)
include(${ODB_USE_FILE})

set(OdbCMake_SOURCES database.h)

set(OdbCMake_ODB_HEADERS account.hpp)

odb_compile(OdbCMake_SOURCES FILES ${OdbCMake_ODB_HEADERS} DB mysql GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA)


add_executable(oppgave2 oppgave2.cpp
        ${OdbCMake_SOURCES}
        ${OdbCMake_ODB_HEADERS})
target_link_libraries(oppgave2
        ${ODB_LIBRARIES})
target_include_directories(oppgave2
        PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_COMPILE_OUTPUT_DIR})
target_compile_definitions(oppgave2
        PRIVATE
        DATABASE_MYSQL)

target_compile_options(oppgave2 PRIVATE -pthread -Wall -Wextra -pedantic -Werror -Wno-reorder -Wno-unused -Wno-unknown-pragmas)

add_executable(oppgave3 oppgave3.cpp
        ${OdbCMake_SOURCES}
        ${OdbCMake_ODB_HEADERS})

target_link_libraries(oppgave3
        ${ODB_LIBRARIES})
target_include_directories(oppgave3
        PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_COMPILE_OUTPUT_DIR})
target_compile_definitions(oppgave3
        PRIVATE
        DATABASE_MYSQL)

target_compile_options(oppgave3 PRIVATE -pthread -Wall -Wextra -pedantic -Werror -Wno-reorder -Wno-unused -Wno-unknown-pragmas)
