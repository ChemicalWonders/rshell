#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{

	int pid = fork();
	string user_stream;
	
	if (pid == 0){
		cout << "I'm a child process. " << endl;
		//PID returns 0 when you have nothing left to execute.
	
		char *argv[3];
trcpy(argv[0], "ls");
                argv[1] = "-a";
                argv[2] = "-l";
                cout << "before" << endl;
                execvp ("ls", argv);
                cout << "after" << endl; // Nothing after execvp will ever be run.
        }
        else{
                wait(NULL);
                cout << "I'm a parent process." << endl;
        }

        return 0;

}	   
