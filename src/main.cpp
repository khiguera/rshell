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
	char line[MAX];
	char *cmd=NULL;		//cmd is used for the parsed string, first parse is the cmd
	char user[256]={0};
	char host[256]={0};
	bool hasQuote=false;
	getlogin_r(user,sizeof(user)-1);
	gethostname(host,sizeof(host)-1);
	while(1)
	{
		//print out prompt and wait for user input

		cout<<user<<"@"<<host<< "$ ";
		fgets(line, MAX, stdin);
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
			if(id==0)
			{
				char* argv[MAX];
				argv[0]=new char[50];
				strcpy(argv[0],cmd);
				cmd=strtok(NULL,DELIMS);
				unsigned int i=0;
				while(cmd!=NULL)
				{
					argv[i+1]=new char[50];
					strcpy(argv[i+1],cmd);
					cmd=strtok(NULL,DELIMS);
					++i;
				}
				argv[i+1]=0;
				int pid =execvp(argv[0],argv);
				if(pid==-1)
					perror("execvp");
				//be sure to exit if execvp failed
				exit(1);
			}
			else if(id==-1)
			{
				perror("fork");
				//be sure to exit if fork failed
				exit(1);
			}
			else
				wait(NULL);
		}
	}

	return 0;
}
