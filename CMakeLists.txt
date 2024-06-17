cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME CMakeConverter)

find_package(Boost)

add_executable(${PROJECT_NAME}
makefile2cmake/main.cpp
makefile2cmake/analysis.cpp
makefile2cmake/fileworker.cpp
makefile2cmake/info.cpp
makefile2cmake/target.cpp
makefile2cmake/workWithStrings.cpp

makefile2cmake/analysis.hpp
makefile2cmake/fileworker.hpp
makefile2cmake/info.hpp
makefile2cmake/target.hpp
makefile2cmake/workWithStrings.hpp)
