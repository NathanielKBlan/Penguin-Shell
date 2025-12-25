cmake_minimum_required(VERSION 4.1)
project(Penguin C)

set(CMAKE_C_STANDARD 23)

add_executable(Penguin main.c
        pen-main/penguin-main.h
        pen-main/penguin-main.c
        antarctic/antarctic_env.h
        antarctic/antarctic_env.c)

target_include_directories(Penguin PRIVATE pen-main antarctic)
