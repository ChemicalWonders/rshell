#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <ncurses.h>
int att=0;



#define LONG 0
#define REC 1
#define ALL 2
#define DIRS 3
#define SIZE 4
#define TIME 5


void handler(char *);
void ls(char* fname);
void long_listing(char* fname);
void ls_recursive(char* fname);
void add_intolist(char *fname);

int num_entry_global_lsbasic=0;
int opt[6];
long total=0;
int duped=0;
int count=0;
char file_name[1000];
void long_listing(char* fname);
int flag=0;
void ls_recursive(char* fname);


//this structure is used to store the elements name with absolute paths
struct direntry
{
	char *dir_name;
	struct direntry *next;
}*head=NULL;



//Inserts elements into that structure for recursive calls

void insert(char fname[1000])
{
	if(head!=NULL)
  	{
    		
		struct direntry (*tmp)=head;
		while(tmp->next!=NULL)
     			 tmp=tmp->next;
		//printf("here \n");
		struct direntry *temp = (struct direntry *)malloc(sizeof(struct direntry));
   		 temp->dir_name=(char *)malloc((strlen(fname)+1)*sizeof(char));
   		 strcpy(temp->dir_name,fname);
   		 temp->next = NULL;
   		 tmp->next=temp;
  	}
  	else
  	{
   		 //printf("first time\n");
		 head =(struct direntry *)malloc(sizeof(struct direntry)) ;
   		 head->dir_name=(char *)malloc((strlen(fname)+1)*sizeof(char));
   		 strcpy(head->dir_name,fname);
   		 head->next = NULL;

  	}

}
//This method override's the sorting order of scandir and get
int alpha_sort (const struct dirent **d1,const struct dirent **d2)
{
        return(strcasecmp((*d1)->d_name,(*d2)->d_name));
}
//override the sorting order in time
int sorttime(const struct dirent **d1,const  struct dirent **d2)
{   
    
    	struct stat buff1,buff2;
    	stat((*d1)->d_name,&buff1);
    	stat((*d2)->d_name,&buff2);
    	if(buff1.st_mtime <= buff2.st_mtime)
    	{
		if( buff1.st_mtime==buff2.st_mtime )		
		{	
			return strcasecmp((*d1)->d_name,(*d2)->d_name);
		}
		else
		{
			return 1;
		}    	
	} 
	
	else
   	{
                return -1;
        }
   
}

// This funtion is used in scandir to sort  files on Block Size
int sortsize(const struct dirent **d1, const struct dirent **d2)
{
  	struct stat entry1,entry2;
  	lstat((*d1)->d_name,&entry1);
  	lstat((*d2)->d_name,&entry2);
	if(entry1.st_size<=entry2.st_size)
	{
		if(entry2.st_size == entry1.st_size)
        		return (strcasecmp((*d1)->d_name,(*d2)->d_name));
    	        return 1;
   	}else
        	return -1;
}

//Basic Ls functionality implemented  with options looking !
void ls(char* filename)
{
	struct dirent **namelist;
	long long int num_entries;
	char *name=(char *)malloc(1000*sizeof(char));
	int i=0,j=0;
	
	if(strcmp(filename,"/")!=0)	
	{	
		strcpy(name,filename);
		strcat(name,"/");
	}else
	{
		strcpy(name,filename);
	}
	strcpy(file_name,name);
	if(opt[TIME]==1)
	{
		//scandir on sorttime function
		num_entries=scandir(name,&namelist,0,sorttime);
    		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
      			return;
    		}

	}else if(opt[SIZE]==1)
	{
		//scandir on sortsize function
		num_entries=scandir(name,&namelist,0,sortsize);
    		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
      			return;
    		}
	}else
	{
		num_entries=scandir(name,&namelist,0,alpha_sort);
		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
			return;
    		}
	}
	if(opt[LONG]==1)
	{
		for(i=0;i<num_entries;i++)
		{
			char *fi=(char *)malloc(1000*sizeof(char));			
			strcpy(fi,name);			
			char c=namelist[i]->d_name[0];			
			if(c=='.')
			{
				if(opt[ALL]==1)
				{	
					char *s=(char *)malloc(1000*sizeof(char));				
					strcpy(s,filename);
					strcat(s,"/");
					strcat(s,namelist[i]->d_name);					
					if(strcmp(namelist[i]->d_name,".")!=0 && strcmp(namelist[i]->d_name,"..")!=0)					
						long_listing(s);
					printf("\n");
					continue;				
				}	
				
			}
			else
			{
				char *s=(char *)malloc(1000*sizeof(char));				
				strcpy(s,filename);
				strcat(s,"/");
				strcat(s,namelist[i]->d_name);	
				long_listing(s);
				printf("\n");
			}
		}
	}
	else if(opt[REC]==1)
	{
		for(i=0;i<num_entries;i++)		
		{
			int flag=0;			
			char *s=(char *)malloc(1000*sizeof(char));	
			if(strcmp(filename,"/")!=0)	
			{	
				strcpy(s,filename);
				strcat(s,"/");
			}else
			{
				strcpy(s,filename);
			}
						
			strcat(s,namelist[i]->d_name);
			struct stat sb;
  			if (lstat(s, &sb) == -1)
  			{
   	 				perror(s);
    					printf("\n");
    					return;
  			}
			if( (sb.st_mode & S_IFMT)==S_IFDIR)
    			{	
				flag=1;				
				if(strcmp(namelist[i]->d_name,".")!=0 && strcmp(namelist[i]->d_name,"..")!=0)
				{
					insert(s);
				}
			}if(flag)
			{
				if(att)				
				{
					printf("\033[22;34m%-30s\033[01;0m",namelist[i]->d_name);
					printf("\n");
				}else
				{
					printf("%-30s",namelist[i]->d_name);
					printf("\n");
				}			
			}else
			{
					printf("%-30s",namelist[i]->d_name);
					printf("\n");
				}
		}
	}
	else
	{
		for(i=0;i<num_entries;i++)
		{
			char *fi=(char *)malloc(1000*sizeof(char));
			strcpy(fi,name);			
			char c=namelist[i]->d_name[0];			
			if(c=='.')
			{
				if(opt[ALL]==1)
				{	
					char *s=(char *)malloc(1000*sizeof(char));				
					strcpy(s,filename);
					strcat(s,"/");
					strcat(s,namelist[i]->d_name);	
					if(opt[REC]==0)			
						printf("%s ",basename(s));					
					else
					{
						if(att)
						{	
							printf("\033[22;34m %-30s \033[01;0m",basename(s));
						}else
						{
	
							printf("%-30s",basename(s));
						}					
					}
					continue;				
				}	
				
			}
			else
			{
						
				char *s=(char *)malloc(1000*sizeof(char));				
				strcpy(s,filename);
				strcat(s,"/");
				strcat(s,namelist[i]->d_name);	
				struct stat sb;
  				if (lstat(s, &sb) == -1)
  				{
   	 				perror(s);
    					printf("\n");
    					return;
  				}
				if( (sb.st_mode & S_IFMT)==S_IFDIR)
    				{	
					flag=1;					
				}
				if(flag==1)
				{	
					if(opt[REC]==0)			
						printf("%s ",basename(s));					
					else
					{
						if(att)
						{						
							printf("\033[22;34m %-30s \033[01;0m",basename(s));
						}
						else
							printf("%-30s",basename(s));				
					}//printf("\033[22;34m %s \033[01;0m",basename(s));
					flag=0;
				}				
				else
					if(opt[REC]==0)			
						printf("%s ",basename(s));					
					else
					{
						if(att)
						{						
							printf("\033[22;34m %-30s \033[01;0m",basename(s));
						}else
							printf("%-30s",basename(s));
					//printf("%s ",basename(s));
					}
			}
		}
		//num_entry_global_lsbasic=num_entries;
		//print_cursor();
	}
	
}



//Adds the directories into the list
void add_intolist(char *filename)
{
	struct dirent **namelist;
	long long int num_entries;
	char *name=(char *)malloc(1000*sizeof(char));
	strcpy(name,filename);
	strcpy(file_name,name);
	int i=0;	
	if(opt[TIME]==1)
	{
		//scandir on sorttime function
		num_entries=scandir(name,&namelist,0,sorttime);
    		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
      			return;
    		}

	}else if(opt[SIZE]==1)
	{
		//scandir on sortsize function
		num_entries=scandir(name,&namelist,0,sortsize);
    		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
      			return;
    		}
	}else
	{
		num_entries=scandir(name,&namelist,0,alpha_sort);
		if(num_entries<0)
    		{
      			printf("Could not scan directory %s\n",name);
			return;
    		}
	}  	
 	for(i=0;i<num_entries;i++)
  	{
    		char *concat=(char*)malloc(1000*sizeof(char));
    		strcpy(concat,name);
		strcat(concat,"/");		
		strcat(concat,namelist[i]->d_name);
		char c=namelist[i]->d_name[0];
		if(c!='.')
		{
			handler(concat);
			//ls_recursive(concat);
		}
	}
}

//It will print file Information in lines with File_permisiions and XYZ..!!
void long_listing(char* fname)
{
  	struct stat stat_temp;
	int flag;
	int i;  	
	if(lstat(fname,&stat_temp)==-1)
  	{
    		printf("error in stat funtion %s\n",fname);
		return;
	}
	struct group *user_group=getgrgid((long)stat_temp.st_gid);
  	struct passwd *user_name=getpwuid((long)stat_temp.st_uid);
  	long int no_of_links=(long)stat_temp.st_nlink;
  	long file_size=stat_temp.st_size;
	total+=stat_temp.st_blocks;
	int islink=((stat_temp.st_mode & S_IFMT )== S_IFLNK);
	struct tm *time_stamp=localtime(&stat_temp.st_mtime);
	if ((stat_temp.st_mode & S_IFMT)==S_IFDIR)
  	{
    		printf("d");
		insert(fname);
	}
  	else if ((stat_temp.st_mode & S_IFMT)==S_IFLNK)
  	{
    		printf("l");
		flag=1;
  	}
  	else 
   		 printf("-");

  	mode_t val;
	
	val=(stat_temp.st_mode & ~S_IFMT);
	(val & S_IRUSR) ? printf("r") : printf("-");
	(val & S_IWUSR) ? printf("w") : printf("-");	
	(val & S_IXUSR) ? printf("x") : printf("-");
	(val & S_IRGRP) ? printf("r") : printf("-");
	(val & S_IWGRP) ? printf("w") : printf("-");
	(val & S_IXGRP) ? printf("x") : printf("-");
	(val & S_IROTH) ? printf("r") : printf("-");
	(val & S_IWOTH) ? printf("w") : printf("-");
	(val & S_IXOTH) ? printf("x") : printf("-");
	
				
		
	printf(" %3ld ",no_of_links);
  	printf(" %4s %4s ",user_name->pw_name,user_group->gr_name);
  	printf(" %10ld ",file_size);
	char buffer[80];
	strftime(buffer,10,"%b",time_stamp);
  	
	//Date and time 
   	printf(" %4d %s %2d ", time_stamp->tm_year+1900,buffer,time_stamp->tm_mday);
	printf(" %2d:%2d ",time_stamp->tm_hour,time_stamp->tm_min);


	
  	if(strcmp (fname,".")!=0 && strcmp (fname,"..")!=0)
    	{
       		char *base;
		base=basename(fname);
		//printf("%s",base);
        	if(base[0]=='.')
        	{
            		if(opt[ALL]==1)
            		{
                		printf("%s ",base);
            		}
        	}	
        	else
            	{	
			if((stat_temp.st_mode & S_IFMT)==S_IFDIR)
			{			
				if(att)
					printf("\033[22;34m%s\033[01;0m",base);
				else
					printf("%s",base);
			}			
			else if(flag==1)
				{
					if(att)				
					{	printf("\033[01;34m %s \033[01;0m",base);				
						flag=0;				
					}else
					{
						printf("%s",base);				
						flag=0;				

					}	
				}			
			else
				printf("%s",base);		
		}        	
		//End if its not a link
        	if(islink==0);
        	else
        	{
            		char *target=(char *)malloc(1000*sizeof(char));
            		int successor=readlink((const char*)fname,target,512);
            		if(successor==-1)
                		strcpy(target,"Broken");
            		else
                		target[successor]='\0';
            		if(att)
				printf("\033[01;34m --> %s \033[01;0m",target);
			else
				printf(" -> %s",target);        	
		}
    	}
	else if(strcmp(fname,".")==0 || strcmp(fname,"..")==0)
	{
		if((stat_temp.st_mode & S_IFMT)==S_IFDIR)		
		{	if(att)
				printf("\033[01;34m ->%s \033[01;0m\n",fname);
			else
				printf("->%s",fname);
		}
		else
			printf("%s\n",fname);
	}
	
}




int main(int argc,char *argv[])
{
	
	att=isatty(1);	
	
	//int row,col;
	//getmaxyx(stdscr,row,col);
	//int row,col;
	char *filename=(char *)malloc(1000*sizeof(char));
	filename = getcwd(filename, 1000);
	if(filename==NULL)
	{			
		printf("Error in $PATH\n");
		return 0;	
	}
	if(argc==1)
	{
			
		ls(filename);
		printf("\n");		
		return 0;
	}
	
	int i,j;
	int k=0;
	for(i=1;i<argc;i++)
	{
		//for options !		
		if(argv[i][0]=='-')
		{
			for(j=1;argv[i][j]!='\0';j++)
			{
								
				char ch=argv[i][j];
				switch(ch)
       				{
        				case 'l' : opt[LONG]=1;
						   break;
        				case 'a' : opt[ALL]=1; 
                   				   break;
        				case 'd' : opt[DIRS]=1;
                   				   break;
        				case 'R' : opt[REC]=1; 
                   				   break;
        				case 'S' : opt[SIZE]=1; 
                   				   opt[TIME]=0;
						   //sorts By file_Size
                   				   break;
        				case 't' : opt[TIME]=1;
                   				   opt[SIZE]=0;
						   //sort by file modification time 
                   				   break;
        				default  : printf("The Option %s is yet to . !!\n",argv[i]);
                   				   return (0);
        			}
      			}
		}
		else if(argv[i][0]!='/') 
		 {
			printf("does not work\n");
			return 1;
		}
		else
		{
			k++;	
		}
	}
	
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
	} 
	
	return 0;
}	



void handler(char *x)
{
  	ls_recursive(x);
}

//Add to list
void ls_recursive(char* fname)
{
  	struct stat sb;
  	if (lstat(fname, &sb) == -1)
  	{
   	 	perror(fname);
    		printf("\n");
    		return;
  	}
  	if( (sb.st_mode & S_IFMT)==S_IFDIR)
    		insert (fname);
}
