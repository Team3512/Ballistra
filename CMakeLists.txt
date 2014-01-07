cmake_minimum_required(VERSION 2.8)
include(FRCDeploy)

set(NAME "2014-robot")

project(${NAME})
find_package(WPILib)
include_directories(${WPILib_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-O3 -Wall -s -std=c++11 -D__VXWORKS__")
set(CMAKE_C_FLAGS "-O3 -Wall -s -std=c11 -D__VXWORKS__")

set(DIRS
  Logging/*
)

file(GLOB SRC *.cpp *.c)

foreach(ELEM ${DIRS})
    file(GLOB_RECURSE FILES ${ELEM}.cpp ${ELEM}.c)
    list(APPEND SRC ${FILES})
endforeach(ELEM)

add_executable(${NAME}.out ${SRC})

target_link_libraries(${NAME}.out ${WPILib_LIBRARY})

add_make_deploy(${NAME}.out 10.35.12.2)
