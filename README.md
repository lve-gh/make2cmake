# make2cmake

Для работы над проектом необходимы git и CMake.

Как собрать проект:

1) Установить папку с проектом
2) В установленной папке октрыть git
3) Прописать команды:
    
mkdir build

cd build/

cmake ..


В папке examples качестве примера предлагается конвертировать следующий makefile:

    CC=g++
    
    CFLAGS=-c -Wall
    
    all: hello
    
    hello: main.o factorial.o hello.o
    	$(CC) main.o factorial.o hello.o -o hello
    
    main.o: main.cpp
    	$(CC) $(CFLAGS) main.cpp
    
    factorial.o: factorial.cpp
    	$(CC) $(CFLAGS) factorial.cpp
    
    hello.o: hello.cpp
    	$(CC) $(CFLAGS) hello.cpp
    
    clean:
    	rm -rf *.o hello

Результатом работы конвертера является следующий файл CMakeLists.txt: 

    cmake_minimum_required(VERSION 3.12)
    
    project(projectName)
    set(CC g++)
    
    set(CFLAGS -c -Wall)
    
    target_link_libraries(all  hello)
    
    target_link_libraries(hello  main.o factorial.o hello.o)
    
    add_executable(main.o  main.cpp)
    
    add_executable(factorial.o  factorial.cpp)
    
    add_executable(hello.o  hello.cpp)
    
    target_link_libraries(clean )
