#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

// Definitions
#define LONG 0
#define REC 1
#define ALL 2
#define DIRS 3
#define SIZE 4
#define TIME 5

// Declared Variables:
int pos=0;
int opt[6];

using namespace std;

void ls(char *filename)
{
	const char *dirName = filename;
    DIR *dirp = opendir(dirName);
    if (dirp == NULL)
    	perror("invalid dir");

    dirent *direntp;

    while ((direntp = readdir(dirp)) != NULL)
        cout << direntp->d_name << endl;

        // use stat here to find attributes of file
    int cldir = closedir(dirp);

    if (cldir < 0)
    	perror("closedir");

}

int main(int argc, char* argv[])
{
	
	pos = isatty(1);	

	char *filename = new char[1000];
	filename = getcwd(filename, 1000);

	if(filename == NULL)
	{			
		perror("empty filename");
		return -1;	
	}

	if(argc == 1)
	{
			
		ls(filename);
		cout << endl;

		return 0;
	}
	
	int i,j = 0;

	for(i=1; i < argc; i++)
	{
		// Tracks if the user put in options. It checks for -, and marks that as an option flag.	
		if(argv[i][0] == '-')
		{
			// Checks for all the values before the null termination.
			for(j=1; argv[i][j]!='\0'; j++)
			{
				char ch = argv[i][j];
				switch(ch)
       				{
       					// Checks for the print all case.
        				case 'a' : opt[ALL]=1;
        				//cout << "You have choosen the all case.\n";
                   			break;

       					// Checks for the long
        				case 'l' : opt[LONG]=1;
        				//cout << "You have choosen the long case.\n";
        					break;

                   		// Checks for the recursive case.
        				case 'R' : opt[REC]=1;
        				//cout << "You have choosen the recursive case.\n"; 
                   			break;

                   		// Returns -1, the program never completed.
        				default  : 
        				cout << "illegal option " << ch << endl;
        				cout << "usage: -alR \n";
        					return -1;
        			}
      		}
		}

		else if(argv[i][0] == '.') 
		{
			printf("you have hidden files\n");
			return -1;
		}
	}
	/*
	//Do things for the current directory !	
	if(k==0)
	{
		if(opt[DIRS]==1)
      		{
        		if(opt[LONG]==1)
        		{
          			long_listing(".");
          			printf("\n");
        		}else
			{	
				printf(".\n");
				return 0;
			}		
		}
		else  		
		{
			if(opt[LONG]==1)			
			{
				if(opt[REC]==1)
					ls(filename);
				else if(opt[ALL]==1)
				{
					ls(filename);
				}else
				{
					ls(filename);
					printf("total : %ld\n",total/2);
				}
    			}
			else if(opt[REC]==1)
			{
				ls(filename);
					
			}else
				ls(filename);	
		}
		if(opt[REC] == 1)
    		{
      			struct direntry *tmp=head;
      			opt[REC] =0;
			while(tmp!=NULL)
      			{
        			printf("-------------------------------------------------\n");
				printf("%s :",tmp->dir_name);
				printf("\n-------------------------------------------------\n");
        			if(opt[LONG]!=1)
         		 		add_intolist(tmp->dir_name);
        			ls(tmp->dir_name);
        			tmp=tmp->next;
       			}
			printf("\n");
		}
		
			
	}
	// If the User has given arguements
	else 
	{
		//call ls on all the stored files
		for(j=1;j<argc;j++)
		{
						
			if(argv[j][0]!='-' && k>=0)
			{
				k--;				
				if(opt[DIRS]==1)  
            			{
                			if(opt[LONG]==1)
                			{
						long_listing(argv[j]);
						printf("\n");
						printf("total : %ld\n",total);
						printf("\n");
                    			}
                			else
					{
                    				printf("%s\n",argv[j]);
						printf("\n");
                			}			
					return (0);
            			}
				struct stat sb;
				printf("%s\n",argv[j]);
            			if (lstat(argv[j], &sb) == -1)
            			{
                			printf("Couldnt stat %s\n",argv[j]);
                			return (1);
            			}
				if((sb.st_mode & S_IFMT)==S_IFDIR)
            			{
                			ls(argv[j]);
					if(opt[LONG]==1)
					printf("total : %ld\n",total);
					printf("\n\n");
					
            			}
            			else if(opt[LONG]==1)
            			{
	                		long_listing(argv[j]);
					printf("total : %ld\n",total);
					printf("\n\n");
               
            			}else if(opt[REC]==1)
				{
					ls(argv[j]);					
				}
				if(opt[REC] == 1)
    				{
      					struct direntry *tmp=head;
      					opt[REC] =0;
					while(tmp!=NULL)
      					{
        					printf("----------------------------------------------------------\n");
        					printf("%s :",tmp->dir_name);
        					printf("\n----------------------------------------------------------\n");
						
        					if(opt[LONG]!=1)
         		 				add_intolist(tmp->dir_name);
        					ls(tmp->dir_name);
        					tmp=tmp->next;
       					}
					printf("\n");
    				}
        				
			}		
		}		
	} */
	
	return 0;
}	