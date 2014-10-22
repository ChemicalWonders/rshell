#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


/* 
// Deprecated function call.
int sizefunc(char* commandstream, const char* delim){
        int i;
        char* p = strtok(commandstream, delim);

        for (i = 0; p!= NULL; p = strtok(NULL, delim) ){
                ++i;
        }
        return (i+1);

}
*/

bool checkforexit(char ** &commandList, char* commands, const char* pk){
        int c = 0;
        bool exiting = false;
        char* par = strtok(commands, pk);

        for (c = 0; par != NULL; ++c, par = strtok(NULL, pk)){
                
                commandList[c] = par;
        }
        return exiting;
                //cout << "CommandList@" << it << ": " << commandList[it] << endl;        }
}

char ** changeToArray(char ** commandList, char* commands, const char* parser){
        int it = 0;
        char* pkey = strtok(commands, parser);

        for (it = 0; pkey != NULL; ++it, pkey = strtok(NULL, parser)){
		commandList[it] = pkey;

                //cout << "CommandList@" << it << ": " << commandList[it] << endl;
        }
        //cout << "it is @ " << it << endl;
        commandList[it] = NULL;
        return commandList;
}

int main()
{       
        string user_stream;
        char * parsedString = NULL;
        char * username = getlogin();

        const char * parr = " ";
        //const char * andL = "&&";
        //const char * orL =  "||";
        //const char * semiL = ";";

        char host[100];
        gethostname(host, 100);

        //Introductory Message to make sure shell is running.
        cout << "Hello, and welcome to Kevin's shell." << endl;

        while(1){

                // Prints out the username and hostname if it exists.
                if (username){
                        cout << username << "@" << host;
                }
                cout << "$ ";

                //Gets line for user stream.
                getline(cin, user_stream);

                if (user_stream == "exit"){
                        cout << "Thank you for using the program.\n";
                        exit(1);
                }

                int hashpos = user_stream.find('#');
                
                if (hashpos != -1){
                       user_stream.erase(hashpos);
                }

                //Parsing the string using strdup to change from const char* to char*
                parsedString = strdup(user_stream.c_str());

		char ** command = new char *[sizeof(parsedString)+1];

                command = changeToArray(command, parsedString, parr);

                int pid = fork();

                if (pid == -1){
                	perror("Fork failed");
		}
                else if (pid == 0){
			
                        if(execvp( (const char*) command [0] , (char* const*) command) == -1){
                                perror("Unable to perform request.");
                        }
		
                }
                else{
                        wait(NULL);

                        /*
                        if (checkforexit(commandstream, parsedString, parr)){
                                cout << "Thank you for using rshell. Goodbye.\n";
                                exit(1);
                        }
                        */
                }


        }

        return 0;

}	   
