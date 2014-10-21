#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

int main()
{       
        string user_stream;
        char * commands;
        //char * username = getlogin();

        char host[1024];
        gethostname(host, 1024);

        //Introductory Message to make sure shell is running.
        cout << "Hello, and welcome to Kevin's shell." << endl;

        getline(cin, user_streaming);

        user_stream = user_streaming.c_str();

        tokenizer<> tok(user_stream);

        for(tokenizer<>::iterator it=tok.begin(); it!=tok.end(); it++){
                cout << *it << "\n";
        }
      /*
        while(1){

                //int pid = fork();
                // Prints out the username and hostname if it exists.
                if (username && host){
                        cout << username << "@" << host;
                }

                cout << "$ ";
                getline(cin, user_stream);

                if (user_stream == "exit"){
                        cout << "Shell will be terminated now." << endl; 
                        exit(0);
                }
        

                //if (pid == 0){
                //cout << "You have typed something in. What do I do now?" << endl;
                //cout << "I'm a child process. " << endl;
                //PID will return 0 when you have nothing left to execute.


                //}

        }
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
