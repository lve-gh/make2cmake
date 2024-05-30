all: target1 target2
		gcc -o target1 target2


target1: target2.o main.cpp
        gcc -o target target2.o main.cpp
		
target2: hello.cpp
		gcc -c hello.cpp