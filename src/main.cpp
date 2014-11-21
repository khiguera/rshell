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
void piping(char in[],int count,int cmdnum)
{
	unsigned pos=0;
	int pipefd[2];
	int fd;
	char *str=NULL;
	char leftPart[MAX];
	char rightPart[MAX];
	for(unsigned i=0;in[i]!='|';++i)
	{
		leftPart[i]=in[i];
		++pos;
	}
	++pos;
	for(unsigned i=0;in[i]!='\0';++i)
	{
		rightPart[i]=in[pos];
		++pos;
	}
	if(cmdnum!=count)
	{
		fd=pipe(pipefd);
		if(fd==-1)
		{
			perror("pipe");
			exit(1);
		}
	}
	int id=fork();
	if(id==0)//in child process
	{
		for(int u=0;u<2;++u)
		{
		str=strtok(leftPart,DELIMS);
		char* argv[MAX];
		argv[0]=new char[50];
		strcpy(argv[0],str);
		str=strtok(NULL,DELIMS);
		unsigned int i=0;
		while(str!=NULL)//tokenize and set arguments for execvp
		{
			argv[i+1]=new char[50];
			strcpy(argv[i+1],str);
			str=strtok(NULL,DELIMS);
			++i;
		}
		argv[i+1]='\0';//add null terminating character
			if(cmdnum==0)
			{
				if(close(1)==-1)
					perror("close");
				if(dup(fd)==-1)
					perror("dup");
			}
			else
			{
				if(close(0)==-1)
					perror("close");
				if(dup(fd)==-1)
					perror("dup");
			}
			++cmdnum;
			for(unsigned o=0;rightPart[o]!='\0';++o)
				leftPart[o]=rightPart[o];
		int pid =execvp(argv[0],argv);
		if(pid==-1)
		{//exit if execvp failed
			perror("execvp");
			exit(1);
		}
		}
	}
	else if (id==-1)
	{
		perror("fork");
		exit(1);
	}
	else
	{
		for(int u=0;u<2;++u)
		{
		if(wait(NULL)==-1)
		{
			perror("wait");
			exit(1);
		}
		}
	}
}
int main()
{
	char line[MAX];		//line is used for input
	char *cmd=NULL;		//cmd is used for the parsed string, first parse is the cmd
	char user[256];		
	char host[256];
	bool hasQuote=false;
	int pipeCnt=0;
	bool hasPipe=false;
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
			if(line[j]=='|'||line[j]=='<'||line[j]=='>')
			{
				hasPipe=true;
				++pipeCnt;
			}
		}
		if(hasPipe)
		{
			piping(line,pipeCnt,0);
			hasPipe=false;
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
