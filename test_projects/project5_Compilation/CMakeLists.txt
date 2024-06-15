cmake_minimum_required(VERSION 3.02)
project(projectName)
add_custom_command(COMMAND python ARGS ..\\./generate_src.py OUTPUT ..\\src.cpp)
add_executable(general src.cpp)