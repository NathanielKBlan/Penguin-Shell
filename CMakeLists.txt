cmake_minimum_required(VERSION 4.1)
project(penguin C)

set(CMAKE_C_STANDARD 23)

add_executable(penguin main.c
        pen-main/penguin-main.h
        pen-main/penguin-main.c
        antarctic/antarctic_env.h
        antarctic/antarctic_env.c)

target_include_directories(penguin PRIVATE pen-main antarctic)
