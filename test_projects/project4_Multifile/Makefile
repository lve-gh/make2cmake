general: hellomake hellomake2
	gcc -o general hellomake hellomake2

hellomake: hellomake.c hellofunc.c
	gcc -o hellomake -c hellomake.c hellofunc.c 

include Makefile2

clean:
	rm -f *.o hellomake Makefile
	mv Makefile.orig Makefile
