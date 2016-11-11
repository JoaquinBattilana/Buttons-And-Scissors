cmake_minimum_required(VERSION 3.6)
project(TPE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")

set(SOURCE_FILES main.c buttonsAndScissorsBack.c buttonsAndScissorsBack.h random.c random.h buttonsAndScissorsFront.c buttonsAndScissorsFront.h)
add_executable(TPE ${SOURCE_FILES})