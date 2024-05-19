# Makefile - Version 1

# Regular compilation command, just as it would be written on
# the command line
include Makefile2

hellomake: hellomake.c hellofunc.c
	gcc -o hellomake hellomake.c hellofunc.c

# Target to clean files created during compilation
clean:
	rm -f *.o hellomake Makefile
	mv Makefile.orig Makefile
