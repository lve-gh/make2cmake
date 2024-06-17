general: src

src: src.o
	g++ src.o -o src

src.o: src.cpp
	g++ -c src.cpp

src.cpp:
	python ./generate_src.py > src.cpp

