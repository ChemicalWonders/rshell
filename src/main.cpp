#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main()
{       
        string user_stream;
        const char * commands;
        char * parsed;
        const char * parser = " ;";
        char * username = getlogin();

        char host[1024];
        gethostname(host, 1024);

        //Introductory Message to make sure shell is running.
        cout << "Hello, and welcome to Kevin's shell." << endl;

        while(1){

                // Prints out the username and hostname if it exists.
                if (username && host){
                        cout << username << "@" << host;
                }
                cout << "$ ";

                //Gets line for user stream.
                getline(cin, user_stream);

                //Parsing the string using strdup to change from const char* to char*
                parsed = strdup(user_stream.c_str());

                //Tokenizes the first one in the array.
                commands = strtok(parsed, parser);

                
                /* walk through to check if commands are null terminated.
                while( commands != NULL ) 
                {
                        printf( " %s\n", commands );
                        commands = strtok(NULL, parser );
                }
                

                if (user_stream == "exit"){
                        cout << "Shell will be terminated now." << endl; 
                        exit(0);
                }
                */

                //Creating a fork process
                int pid = fork();
        

                if (pid == -1){

                        //PID will return -1 when an error occurs.
                        cout << "Fork broke. Shell will now exit." << endl;
                        exit(1);
                }

        }
        /*
	if (pid == 0){
		cout << "I'm a child process. " << endl;
		//PID returns 0 when you have nothing left to execute.
	
		char *argv[3];
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
        }*/

        return 0;

}	   
