cmake_minimum_required(VERSION 3.30)
project(enseashell C)

set(CMAKE_C_STANDARD 11)

add_executable(enseashell main.c
        shell.c
        utils.c
        utils.h
        shell.h
        time_utils.c
        time_utils.h
        redirection.c
        redirection.h
        pipe.c
        pipe.h)
