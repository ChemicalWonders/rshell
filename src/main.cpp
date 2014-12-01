#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

//Checks for username and hostname
void setUsernameHostname();
// Finds and erases everything past the comment
void removeComment(string &);
//Exit program
void exitProgram(char stream []);
// Parses, and references the amount of commands
int changeToArray(char ** commandList, char* commands, bool& emptyinput);
// Checks for background processes
bool background(int num, char ** commandList, bool emptyUI);
// Checks for a pipe, and then cleans it out
void cleanpipe(char** commandList, bool background);
// Executes the pipe
void executeWpipe(char** part1, char** part2, bool background);
// Duplicates the file so that the pipe is able to process anything in it.
void fileDuplicate(char ** commandList);
// Old execute - execvp + wait with background process
void execute(char ** commandList);

void setUsernameHostname()
{
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

void removecomment(string &stream)
{
    int hashpos = stream.find('#');

    //Check for comments, and removes anything past #
    if (hashpos != -1){
        stream.erase(hashpos);
    }
}

void exitprogram(char stream[])
{
    if(!strcmp(stream, "exit"))
        exit(1);
}

int changeToArray(char ** commandList, char* command)
{
    int argnum = 0;
    commandList[argnum] = strtok(command," \n\t");

    while(commandList[argnum] != NULL)
    {
        argnum++;
        commandList[argnum] = strtok(NULL, " \n\t");
    }
    return argnum;
}

void cleanpipe(char ** commandList, bool background)
{
    bool pipeExist = false;
    int pipeloc = 0;
    char** pipe1 = new char *[50];
    char** pipe2 = new char *[50];


    for(int i = 0; commandList[i]!= '\0'; ++i)
    {
        if(!strcmp(commandList[i], "|"))
        {
            pipeExist = true;
            pipeloc = i;
            break; 
        }
    }
    // If pipe exists, then I break it up into 2
    if(pipeExist)
    {
        int pipe2index = 0;
        int i = 0;
        for(; i < pipeloc; ++i)
            pipe1[i] = commandList[i];

        pipe1[pipeloc] = NULL;

        for(int i=pipeloc+1; commandList[i] != '\0'; i++)
        {
            pipe2[pipe2index] = commandList[i];
            ++pipe2index;
        }
        
        pipe2[pipe2index] = NULL;

        executeWpipe(pipe1, pipe2, background);
    }
    else
    {
        execute(commandList);
    }

    delete [] pipe1;
    delete [] pipe2;
}

void executeWpipe(char** pipe1, char** pipe2, bool background)
{
    int fd[2];
    if(pipe(fd) == -1)
        perror("execpipe.pipe");
    
    int pid = fork();
    if(pid == -1)
    {
        perror("execpipe.close");
    }

    //start the first process in the pipe.
    else if(pid == 0)
    {
        if(-1 == dup2(fd[1],1))
            perror("execpipe.dup2");

        if(-1 == close(fd[0]))
            perror("execpipe.close");

        if(-1 == execvp(pipe1[0], pipe1))
            perror("execpipe.execvp");

        exit(1);
    }
    
    // read in from the pipe
    int savestdin = 0;

    if(-1 == (savestdin = dup(0)))
        perror("execpipe.dup");

    if(-1 == dup2(fd[0], 0))
        perror("execpipe.dup2");

    if(-1 == close(fd[1]))
        perror("execpipe.close");

    if(-1 == wait(0))
        perror("execpipe.wait");

    // Required to check for another pipe in the second half.
    cleanpipe(pipe2, background);

    if(dup2(savestdin, 0) == -1)
        perror("fd.dup2>>");

    //IF I ADD THE FFLUSH, IT BREAKS, but I need to be able to clean out the buffers.
    //fflush(NULL);
}

bool background(int num, char ** commandList, bool emptyUI)
{
    bool backproc = false;
    //check for background processes
    if(num > 0) 
        --num;
    // Checks for string comparison to & and tries to find it.
    if(!emptyUI && !strcmp(commandList[num],"&"))
    {
        backproc = true;
        commandList[num] = NULL;
    }
    else if(!emptyUI)
    {
        char *copy = commandList[num];
        int len = strlen(commandList[num]) - 1;
        if(copy[len] == '&') 
        {
            backproc = true;
            copy[len] = '\0';
            commandList[num] = copy;
        }
    }
    return backproc;
}

void fileDuplicate(char ** commandList)
{
    for(int i = 0 ; commandList[i] != '\0'; ++i)
    {
        int fd = 0;
        if(!strcmp(commandList[i], ">"))
        {
            commandList[i] = NULL;
            if((fd = open(commandList[i + 1],  O_WRONLY | O_CREAT | O_TRUNC , 0666 )) == -1)
                perror("fd.open>");

            if(dup2(fd, 1) == -1)
                perror("fd.dup2>");

            break;
        }

        else if(!strcmp(commandList[i], ">>"))
        {
            commandList[i] = NULL;
            if((open(commandList[i + 1], O_WRONLY| O_CREAT | O_APPEND, 0666 )) == -1)
                perror("fd.open>>");

            if(dup2(fd, 1) == -1)
                perror("fd.dup2>>");

            break;
        }

        else if(!strcmp(commandList[i], "<"))
        {
            commandList[i] = NULL;
            if((fd = open(commandList[i+1], O_RDONLY )) == -1)
                perror("fd.open<");

            if(dup2(fd, 0) == -1)
                perror("fd.dup2<");

            break;
        }
    }
}

void execute(char ** commandList)
{
    int pid = fork();

    if(pid == -1)
        perror("exec.fork");

    else if(pid == 0)
    {
        fileDuplicate(commandList);

        if(execvp(commandList[0], commandList) == -1)
            perror("exec.execvp");

        exit(1);
    }
    else
    {
        if(wait(0) == -1) 
            perror("exec.wait");
    }
}

//int newexecvp()
//{

//}

void sighandler(int signum)
{
    if(signum == SIGINT)
    {
        signal(SIGINT, SIG_IGN);
    }
}

//void changedirectory[]
//{

//}

int main()
{

    //Introductory Message to make sure shell is running.
    //cout << "Hello, and welcome to Kevin's shell." << endl;
    //Main function is all full of functions. yay

     //handles control c
     signal(SIGINT, sighandler);

    while(1)
    {
        // Initalized Variables for everything to work.
        char userInput[1000];
        char ** commandList = new char* [50];
        string commands;
        bool emptyUI = false;
        bool backproc = false;

        // Sets Username and Hostname if possible
        setUsernameHostname();

        // getline returns string, so commands is set to a string
        getline(cin, commands);
        removecomment(commands);
        
        //changes commands into a cstring then copies into userInput
        strcpy(userInput, commands.c_str());

        // Checks for exit, if exit exists, then exit the program
        exitprogram(userInput);


        //takes care if user pressed enter
        if(!strcmp(userInput,""))
            emptyUI = true;

        //parsing input
        int parlist = changeToArray(commandList, userInput);

        //check if anything needs to be processed in the background
        backproc = background(parlist, commandList, emptyUI);

        //Checks for piping
        cleanpipe(commandList,backproc);
 
      delete [] commandList;
    }

    return 0;
}
