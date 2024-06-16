cmake_minimum_required(VERSION 3.29)
project(projectName)
add_custom_command(OUTPUT src.cpp COMMAND python ARGS ..\\./generate_src.py)

add_executable(all  src.cpp )

