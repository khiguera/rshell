#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <iostream>

using namespace std;

int main(int  argc, char *const argv[])
{
	int pid=fork();
	if(pid==0)
	{
		cout << "child" <<endl;
		
		execvp("ls",argv);
	}
	else
	{
		wait(NULL);
		cout << "parent"<<endl;
	}
	return 0;
}

