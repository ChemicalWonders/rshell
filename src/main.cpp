#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


// Changes userstream to an array to be parsed
char ** changeToArray(char ** commandList, char* commands, const char* parser);
//Checks for username and hostname
void setUsernameHostname();
// Finds and erases something if needed to be
void removeComment(string &);
//Exit program
void exitProgram(string);


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

void setUsernameHostname(){

    char * username = getlogin();
    if (username == NULL){
        perror("getlogin");
    }
    char host[100];
    if (-1 == gethostname(host, 100))
        perror("gethostname");

    // Prints out the username and hostname if it exists.
    if (username){
        cout << username << "@" << host;
    }
    cout << "$ ";
}

void removeComment(string &stream){
    int hashpos = stream.find('#');

    //Check for comments, and removes anything past #
    if (hashpos != -1){
        stream.erase(hashpos);
    }

}

void exitProgram(string stream){
    int exitpos = stream.find("exit");

    // Checks for exit word, and exits program if found
    if (exitpos != -1){
        exit(0);
    }
}

int main()
{       
        string user_stream;
        char * parsedString = NULL;

        const char * parr = " ";
        //const char * andL = "&&";
        //const char * orL =  "||";
        //const char * semiL = ";";



        //Introductory Message to make sure shell is running.
        //cout << "Hello, and welcome to Kevin's shell." << endl;

        while(1){
            // Sets Username and Hostname if possible
            setUsernameHostname();

            //Gets line for user stream.
            getline(cin, user_stream);

            // Removes all the comments before continuing.
            removeComment(user_stream);

            //Exits program if exit is found.
            exitProgram(user_stream);

            //Parsing the string using strdup to change from const char* to char*
            parsedString = strdup(user_stream.c_str());

	        char ** command = new char *[sizeof(parsedString)+1];
            command = changeToArray(command, parsedString, parr);
            
            int pid = fork();
            if (pid == -1)
            {
            	perror("Fork failed");
	        }

            else if (pid == 0)
            {
                if(execvp((const char*) command[0], (char* const*)command) == -1){
                    perror("Unable to perform request");
                }
	
            }

            else
            {
                if(-1 == wait(NULL))
                    perror("wait");
            }

        }
        return 0;

}	   
