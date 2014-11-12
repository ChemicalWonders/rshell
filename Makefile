# This is the Makefile for rshell
opt:= -Wall -Werror -ansi -pedantic
compiler := g++
objdir := bin

all: bin rshell ls  cp

bin:
	test -d $(objdir) || mkdir $(objdir)
rshell: 
	$(compiler) ./src/main.cpp $(opt) -o ./bin/rshell

ls:
	$(compiler) ./src/ls.cpp   $(opt) -o ./bin/ls

#when cp is added, I will enable
cp:
	$(compiler) ./src/cp.cpp   $(opt) -o ./bin/cp

clean:
	rm -rf $(objdir)
