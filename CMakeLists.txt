cmake_minimum_required(VERSION 4.1)
project(PenguinC C)

set(CMAKE_C_STANDARD 23)

add_executable(PenguinC main.c
        pen-main/penguin-main.h
        pen-main/penguin-main.c)

target_include_directories(PenguinC PRIVATE pen-main)
