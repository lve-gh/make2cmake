cmake_minimum_required(VERSION 3.29)
project(projectName)
add_library(hellomake OBJECT  hellomake.c hellofunc.c)
add_executable(hellomake2  hellomake2.c )
