#include <unistd.h>
#include <string.h>

#include <iostream>
#include <string>

using namespace std;

int main()
{
	char *argv[3];
	argv[0] = new char[3];
	strcpy(argv[0], "ls");
	argv[1] = "-a";
	argv[2] = "l";
	cout << "before" << endl;
	execvp ("ls", argv);
	cout << "after" << endl; // Nothing after execvp will ever be run.

	return 0;

}
