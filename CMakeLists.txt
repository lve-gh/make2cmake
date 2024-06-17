cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME CMakeConverter)
project(${PROJECT_NAME})

find_package(Boost)

set(${Boost_INCLUDE_DIR} "C:\\local\\boost_1_85_0")

include_directories(${Boost_INCLUDE_DIR})

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

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
