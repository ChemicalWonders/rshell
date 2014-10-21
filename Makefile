# This is the Makefile for rshell
opt:= -Wall -Werror -ansi -pedantic
compiler := g++
objdir := bin

all: 
	test -d $(objdir) || mkdir $(objdir)
	$(compiler) ./src/main.cpp $(opt) -o ./bin/rshell

rshell:
	$(compiler) ./src/main.cpp $(opt) -o ./bin/rshell

clean:
	rm -rf $(objdir)