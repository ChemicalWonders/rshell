#include <stdio.h>
#include <iostream>
#include "Timer.h"
#include <cstdio>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;

void copyandpaste(int argc, char* argv[]){
	char* buf = new char [BUFSIZ];
	int fdi = open(argv[1], O_RDONLY);
        if (access (argv[2], F_OK) != -1){
	   perror("file exist");
           exit(-1);
        }
	int fdo = open(argv[2], O_WRONLY | O_CREAT);
	
	int num;
	if (fdi == -1){
	  perror("fd");
          exit(1);
        }
	if (fdo == -1){
          perror("fd2");
          exit(1);
        }
  while((num = read(fdi, buf, BUFSIZ))){
      if(num == -1){
          perror("read");
      }
      int alpha = write(fdo, buf, num);
      if (alpha == -1){
          perror("write");
      }
  }
  int cdir = close(fdo);
  if (cdir < 0)
    perror("close");
}

int main( int argc, char* argv[])
{
    char method;
    Timer t;
    double eTime;
    double aTime;
    double bTime;

    if (argc == 3){
	copyandpaste(argc, argv);
    }

    else if (argc == 4){
        method = '1'; 

    }
	
    if(method == '1'){
      t.start();
      ifstream filename(argv[1]);
      if (access (argv[2], F_OK) != -1){
	   perror("file exist");
           exit(-1);
        }
      ofstream output(argv[2]);
      
      	while (!filename.eof()){
	   char x;
	   x=filename.get();
	   output.put(x);
	
        }
      t.elapsedTime(aTime, eTime, bTime);
      cout << "Wallclock Time: " << aTime << endl;
      cout << "Elapsed User Time: " << eTime << endl;
      cout << "System Time: " << bTime << endl;
    

      cout << "Method 2, starting ... \n"; 
       
       t.start();
	char* buf2 = new char [BUFSIZ];
	int fdi2 = open(argv[1], O_RDONLY);
	
	int fdo2 = open(argv[2], O_WRONLY | O_CREAT);
        
	int num2;
	if (fdi2 == -1){
	  perror("fd");
          exit(1);
        }
	if (fdo2 == -1){
          perror("fd2");
          exit(1);
        }
        while((num2 = read(fdi2, buf2, 1))){
          if(num2 == -1){
             perror("read");
          }
          int alpha2 = write(fdo2, buf2, num2);
          if (alpha2 == -1){
             perror("write");
          }
	}
      	t.elapsedTime(aTime, eTime, bTime);
      	cout << "Wallclock Time: " << aTime << endl;
      	cout << "Elapsed User Time: " << eTime << endl;
      	cout << "System Time: " << bTime << endl;

	cout << "Method 3, starting ... \n";
	t.start();
	char* buf = new char [BUFSIZ];
	int fdi = open(argv[1], O_RDONLY);
        
	int fdo = open(argv[2], O_WRONLY | O_CREAT);
	
	int num;
	if (fdi == -1){
	  perror("fd");
          exit(1);
        }
	if (fdo == -1){
          perror("fd2");
          exit(1);
        }
        while((num = read(fdi, buf, BUFSIZ))){
          if(num == -1){
             perror("read");
          }
          int alpha = write(fdo, buf, num);
          if (alpha == -1){
             perror("write");
          }
	}
      	t.elapsedTime(aTime, eTime, bTime);
      	cout << "Wallclock Time: " << aTime << endl;
      	cout << "Elapsed User Time: " << eTime << endl;
      	cout << "System Time: " << bTime << endl;
      }
	return 0;
}

	

