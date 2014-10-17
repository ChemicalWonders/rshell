#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	int pid = fork();
	if (pid == 0){
		cout << "I'm a child process." << endl;

		char *argv[3];
		argv[0] = new char[3];
		strcpy(argv[0], "ls");
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
