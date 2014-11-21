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

bool background(int num, char ** commandList, bool emptyUI);

void cleanpipe(char** commandList, bool background);

void executeWpipe(char** part1, char** part2, bool background);

void fileDuplicate(char ** commandList);
// Old execute - execvp + wait with background process
void execute(char ** commandList, bool backproc);

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

int changeToArray(char ** commandList, char* command, bool& emptyinput)
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
    bool pipe = false;
    int pipeloc = 0;

    char** part1;
    part1 = new char*[50];
    char** part2;
    part2 = new char *[50];

    for(int i=0; commandList[i] != '\0';i++)
    {
        if(!strcmp(commandList[i], "|"))
        {
            pipe = true;
            pipeloc = i;
            break; 
        }
    }

    if(pipe)
    {
        for(int i=0; i<pipeloc; i++)
            part1[i] = commandList[i];

        int pt2ind = 0;
        for(int i=pipeloc+1; commandList[i] != '\0'; i++)
        {
            part2[pt2ind] = commandList[i];
            pt2ind++;
        }
        part1[pipeloc] = NULL;
        part2[pt2ind] = NULL;

        executeWpipe(part1,part2,background);
    }
    else//it's a regular command, no pipe
        execute(commandList,background);

    delete[] part1;
    delete[] part2;
}

void executeWpipe(char** part1, char** part2, bool background)
{
    int fd[2];
    if(pipe(fd) == -1)
        perror("execpipe.pipe");
    
    int pid = fork();
    if(pid == -1)
    {
        perror("execpipe.close");
    }
    else if(pid == 0)
    {
        //write to pipe 
        if(-1 == dup2(fd[1],1))
            perror("execpipe.dup2");
        if(-1 == close(fd[0]))
            perror("execpipe.close");
        if(-1 == execvp(part1[0], part1))
        {
            perror("execpipe.execvp");
        }
        exit(1);
    }
    //read from pipe

    int savestdin;
    if(-1 == (savestdin = dup(0)))//need to restore later or infinite loop
        perror("execpipe.dup");

    if(-1 == dup2(fd[0],0))
        perror("execpipe.dup2");
    if(-1 == close(fd[1]))
        perror("execpipe.close");

    if(-1 == wait(0))
        perror("execpipe.wait");


    cleanpipe(part2,background);//in order to chain multiple pipes you have to check again if you have a pipe in the second half

    if(dup2(savestdin,0) == -1)
        perror("fd.dup2>>");

    fflush(NULL);
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
        char* copy = commandList[num];
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
            if((fd = open(commandList[i + 1], O_TRUNC | O_WRONLY | O_CREAT, 0666 )) == -1)
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

void execute(char ** commandList, bool backproc)
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
    if(!backproc)
        if(wait(0) == -1) 
            perror("exec.wait");
}

int main()
{

    //Introductory Message to make sure shell is running.
    //cout << "Hello, and welcome to Kevin's shell." << endl;
    //Main function is all full of functions. yay

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
        int parlist = changeToArray(commandList, userInput, emptyUI);

        //check if anything needs to be processed in the background
        backproc = background(parlist, commandList, emptyUI);

        //Checks for piping
        cleanpipe(commandList,backproc);

       delete [] commandList;
    }

    return 0;
}
