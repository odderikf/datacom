cmake_minimum_required(VERSION 3.10)
project(oving5)
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "/home/odderikf/coding/datacom/oving5/cmake/Modules")
find_package(ODB REQUIRED COMPONENTS mysql)
include(${ODB_USE_FILE})

set(OdbCMake_SOURCES
        oppgave3.cpp
        database.h)

set(OdbCMake_ODB_HEADERS
        account.hpp)

odb_compile(OdbCMake_SOURCES FILES ${OdbCMake_ODB_HEADERS} DB mysql GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA)


add_executable(odbcmake
        ${OdbCMake_SOURCES}
        ${OdbCMake_ODB_HEADERS})
target_link_libraries(odbcmake
        ${ODB_LIBRARIES})
target_include_directories(odbcmake
        PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_COMPILE_OUTPUT_DIR})
target_compile_definitions(odbcmake
        PRIVATE
        DATABASE_SQLITE)