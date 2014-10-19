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
#define DELIMS " \t\r\n"
#define MAX 1000
int main()
{
	char line[MAX];
	char *cmd=NULL;
	while(1)
	{
		//print out prompt and wait for user input
		cout << "$ ";
		if(!fgets(line, MAX, stdin))
			break;
		//parse line for first cmd and see if it is exit
		cmd=strtok(line, DELIMS);

		if(strcmp(cmd,"exit")==0)
			exit(0);
		//if not exit attempt to execute cmd
		else{
			int id=fork();
			if(id==0)
			{
				char* argv[50];
				argv[0]=new char[6];
				strcpy(argv[0],cmd);
				cmd=strtok(NULL,DELIMS);
				unsigned int i=0;
				while(cmd!=NULL)
				{
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
