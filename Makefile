# This is the Makefile for rshell
opt="-Wall -Werror -ansi -pedantic"
compiler="g++"

all:
	mkdir bin
	g++ ./src/main.cpp -o ./bin/rshell

rshell:
	($compiler) ./src/main.cpp ($opt) -o ./bin/rshell

clean:
	rm -rf bin


