# This is the Makefile for rshell
opt="-Wall -Werror -pedantic"
compiler="g++"

all:
	g++ main.cpp -o file.out

clean:
	rm *.out


