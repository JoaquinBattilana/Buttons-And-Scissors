cmake_minimum_required(VERSION 3.6)
project(TPE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES main.c buttonsAndScissorsBack.c buttonsAndScissorsBack.h random.c random.h)
add_executable(TPE ${SOURCE_FILES})