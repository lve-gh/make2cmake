cmake_minimum_required(VERSION 3.02)
project(projectName)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CPP_COMPILER g++)

set(CMAKE_CXX_FLAGS -c -Wall)
set(CMAKE_C_FLAGS -c -Wall)

set(CC g++)
set(CFLAGS -c -Wall)

add_library(hello.o OBJECT hello.cpp)
target_compile_options(hello.o PRIVATE ${CMAKE_CXX_FLAGS})

add_library(factorial.o OBJECT factorial.cpp)
target_compile_options(factorial.o PRIVATE ${CMAKE_CXX_FLAGS})

add_library(main.o OBJECT main.cpp)
target_compile_options(main.o PRIVATE ${CMAKE_CXX_FLAGS})

add_library(hello $<TARGET_OBJECTS:main.o>
$<TARGET_OBJECTS:factorial.o>
$<TARGET_OBJECTS:hello.o> )
target_compile_options(hello PRIVATE ${CMAKE_CXX_FLAGS})


add_executable(general hello)
target_link_libraries(general hello)
target_compile_options(general PRIVATE ${CMAKE_CXX_FLAGS})
