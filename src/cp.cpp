#include <iostream>
#include "Timer.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
using namespace std;


int main(int argc, char *argv[])
{
	char no_flag;
	bool flag=false;
	if(argc<3)
	{
		cout<<"ERROR: Not enough arguments. Missing source and/or destination"<<endl;
	}
	else if(argc==4)
	{
		flag=true;
	}
	else if(argc==3)
	{
		no_flag='3';
	}
	if(!flag&&(access(argv[2],F_OK)!=-1))
	{
		perror("outputfile exists");
		exit(1);
	}
	if(flag)
	{
		Timer t;
		double eTime;
		t.start();
		char *ist=argv[1];
		char *ost=argv[2];
	
		ifstream is(ist);
		ofstream outfile(ost);
		if(is.good())
		{
			
			while(is.good())
			{
				char c=is.get();
				if(is.good())
					outfile.put(c);
					
			}
			cout<<"Method 1"<<endl;
			t.elapsedWallclockTime(eTime);
			cout<<"Wall clock:"<<eTime<<endl;
			t.elapsedSystemTime(eTime);
			cout<<"Sysytem Time:"<<eTime<<endl;
			t.elapsedUserTime(eTime);
			cout<<"User Time:"<<eTime<<endl;
		}
		else
			cout<<"ERROR: could not open file"<<endl;
	}
	if(flag)
	{
		Timer t;
		double eTime;
		t.start();
		char buf[1];
		int num;
		int fdi= open(argv[1],O_RDONLY);
		if(fdi<0)
		{
			perror("open");
			exit(1);
		}
		int fdo =open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
		if(fdo<0)
		{
			perror("open");
			exit(1);
		}
		while((num=read(fdi,buf,1)))		//num used to error check read
		{
			if(num<0)
			{
				perror("read");
				exit(1);
			}
			int num1=write(fdo,buf,num);	//num1 used to error check write
			if(num1<0)
			{
				perror("write");
				exit(1);
			}
		}
		int clsi=close(fdi);		//clsi used to error check closing infile
		if(clsi==-1)
		{
			perror("close in");
			exit(1);
		}
		int clso=close(fdo);		//clso  used to error check closing outfile
		if(clso==-1)
		{
			perror("close in");
			exit(1);
		}
		cout<<"Method 2"<<endl;
		t.elapsedWallclockTime(eTime);
		cout<<"Wall clock:"<<eTime<<endl;
		t.elapsedSystemTime(eTime);
		cout<<"Sysytem Time:"<<eTime<<endl;
		t.elapsedUserTime(eTime);
		cout<<"User Time:"<<eTime<<endl;


		
	}
	if(flag||no_flag=='3')
	{
		Timer t;
		double eTime;
		t.start();
		char buf[BUFSIZ];
		int num;
		int fdi= open(argv[1],O_RDONLY);
		if(fdi<0)
		{
			perror("open");
			exit(1);
		}
		int fdo =open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
		if(fdo<0)
		{
			perror("open");
			exit(1);
		}
		while((num=read(fdi,buf,BUFSIZ)))		//num used to error check read
		{
			if(num<0)
			{
				perror("read");
				exit(1);
			}
			int num1=write(fdo,buf,num);	//num1 used to error check write
			if(num1<0)
			{
				perror("write");
				exit(1);
			}
		}
		int clsi=close(fdi);		//clsi used to error check closing infile
		if(clsi==-1)
		{
			perror("close in");
			exit(1);
		}
		int clso=close(fdo);		//clso  used to error check closing outfile
		if(clso==-1)
		{
			perror("close in");
			exit(1);
		}
		cout<<"Method 3"<<endl;
		t.elapsedWallclockTime(eTime);
		cout<<"Wall clock:"<<eTime<<endl;
		t.elapsedSystemTime(eTime);
		cout<<"Sysytem Time:"<<eTime<<endl;
		t.elapsedUserTime(eTime);
		cout<<"User Time:"<<eTime<<endl;

	}
	

	return 0;

}
