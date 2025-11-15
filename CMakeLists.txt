cmake_minimum_required(VERSION 4.0)
project(bibliotheque_universitaire C)

set(CMAKE_C_STANDARD 23)

add_executable(bibliotheque_universitaire main.c
        librairy.c
        librairy.h
        User_interface.c
        User_interface.h)
