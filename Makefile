# This is the Makefile for rshell
opt="-Wall -Werror"
compiler="g++"

all:
	#[] mkdir bin  #[] gives you if statement
	g++ main.cpp -o file.out
	echo "done"

target2:
	echo "test" && pwd && echo "done"

clean:
	rm *.o *.out

install:

test:


