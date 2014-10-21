#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

char ** changeToArray(char ** commandList, char* commands, const char* parser){
        int i;
        char* parsingKey = strtok(commands, parser);

        for (i = 0; parsingKey != NULL; parsingKey = strtok(NULL, parser) ){
                commandList[i] = parsingKey;
                ++i;
        }

        commandList[i] = NULL;
        return commandList;
}

int main()
{       
        string user_stream;
        const char * parser = " ;";
        char * parsedString;
        char * username = getlogin();
        char ** commandstream = NULL;

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
                parsedString = strdup(user_stream.c_str());

                changeToArray(commandstream, parsedString, parser);

                execvp(parsedString, commandstream);

                
                //walk through to check if commands are null terminated.
                
                /*
                if (user_stream == "exit"){
                        cout << "Shell will be terminated now." << endl; 
                        exit(0);
                }
                */

                //Creating a fork process
                /*
                int pid = fork();
        

                if (pid == -1){

                        //PID will return -1 when an error occurs.
                        cout << "Fork broke. Shell will now exit." << endl;
                        exit(1);
                }
                else if (pid == 0){
                        
                }

                else{
                        wait(NULL);
                        cout << "I'm done." << endl;
                }
                */

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
