#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
#define DELIMS " \r\t\n\""
#define MAX 1024
int main()
{
	char line[MAX];		//line is used for input
	char *cmd=NULL;		//cmd is used for the parsed string, first parse is the cmd
	char user[256];		
	char host[256];
	bool hasQuote=false;
	int log=getlogin_r(user,sizeof(user)-1);	//login
	int hos=gethostname(host,sizeof(host)-1);	//hostname
	if(log!=0)
		perror("getlogin");
	if(hos!=0)
		perror("gethostname");
	while(1)
	{
		//print out prompt and wait for user input
		cout<<user<<"@"<<host<< "$ ";
		fgets(line, MAX, stdin);

		//check for quote or comment and set null terminating accordingly
		for(unsigned j=0;j<MAX;++j)
		{
			if(line[j]=='\"')
				hasQuote=true;
			if(!hasQuote&&line[j]=='#')
				line[j]='\0';
		}

		//parse line for first cmd and see if it is exit
		cmd=strtok(line, DELIMS);
		if(strcmp(cmd,"exit")==0)
			exit(0);
		//if not exit attempt to execute cmd
		else
		{
			int id=fork();
			if(id==0)//in child process
			{
				char* argv[MAX];
				argv[0]=new char[50];
				strcpy(argv[0],cmd);
				cmd=strtok(NULL,DELIMS);
				unsigned int i=0;
				while(cmd!=NULL) //tokenize and set arguments for execvp
				{
					argv[i+1]=new char[50];
					strcpy(argv[i+1],cmd);
					cmd=strtok(NULL,DELIMS);
					++i;
				}
				argv[i+1]='\0'; //add null terminating character
				int pid =execvp(argv[0],argv);
				if(pid==-1) //exit if execvp failed
					perror("execvp");
				exit(1);
			}
			else if(id==-1) //exit if fork failed
			{
				perror("fork");
				exit(1);
			}
			else //in parent process
				wait(NULL);
		}
	}

	return 0;
}
