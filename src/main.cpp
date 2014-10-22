#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

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
        const char* exitword = "exit";

        for (c = 0; par != NULL; ++c, par = strtok(NULL, pk)){
                if (strcmp(exitword, commandList[c])){
                        exiting = true;
                }
                commandList[c] = par;
        }
        return exiting;
                //cout << "CommandList@" << it << ": " << commandList[it] << endl;        }
}

void changeToArray(char ** &commandList, char* commands, const char* parser){
        int it = 0;
        char* pkey = strtok(commands, parser);

        for (it = 0; pkey != NULL; ++it, pkey = strtok(NULL, parser)){

                //cout << "CommandList@" << it << ": " << commandList[it] << endl;
        }
        //cout << "it is @ " << it << endl;
        commandList[it] = NULL;
}

int main()
{       
        string user_stream;
        //char * parsedString = NULL;
        //char * iterate = NULL;
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
                if (username && host){
                        cout << username << "@" << host;
                }
                cout << "$ ";

                //Gets line for user stream.
                getline(cin, user_stream);

                int strlength = user_stream.length();
                int hashpos = user_stream.find("#");

                char parsedString[strlength];


                //Parsing the string using strdup to change from const char* to char*
                strcpy(parsedString, user_stream.c_str());

                parsedString[hashpos] = '\0';


                

                char ** commandstream = new char*[sizeof(parsedString)+1];
                changeToArray(commandstream, parsedString, parr);

                int pid = fork();

                if (pid == -1){
                        cout << "You have taken up all the processes available.\n Please kill some processes before continuing.\n ";
                }
                else if (pid == 0){

                        if(execvp((const char*)commandstream[0], (char* const*) commandstream) == -1){
                                cout << "Unable to perform request." << endl;
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
