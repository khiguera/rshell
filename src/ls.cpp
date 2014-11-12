#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


void noflags()
{
	
	string dirName = ".";
	DIR *dirp = opendir(dirName.c_str());
	if(errno!='\0')
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	 while ((direntp = readdir(dirp)))
	 {
		 if(direntp=='\0')
		 {
			 perror("readdir");
			 exit(1);
		 }
		 if(direntp->d_name[0]=='.')
			 continue;
		else
			cout << direntp->d_name << " ";  // use stat here to find attributes of file
	}
	cout<<endl;
	closedir(dirp);
	if(errno==-1)
	{
		 perror("closedir");
		 exit(1);
	}
}

void aflag()
{
	
	string dirName = ".";
	DIR *dirp = opendir(dirName.c_str());
	if(errno!='\0')
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	 while ((direntp = readdir(dirp)))
	 {
		 if(direntp=='\0')
		 {
			 perror("readdir");
			 exit(1);
		 }
		cout << direntp->d_name <<" ";  // use stat here to find attributes of file
	}
	cout<<endl;
	closedir(dirp);
	if(errno==-1)
	{
		 perror("closedir");
		 exit(1);
	}
}
void printpermissions(struct stat &buf)
{
	cout << endl;
	if(buf.st_mode & S_IFREG)
		cout << '-';
	else if(buf.st_mode & S_IFDIR)
		cout << 'd';
	else if(buf.st_mode & S_IFLNK)
		cout << 'l';
	else if(buf.st_mode & S_IFSOCK)
		cout << 's';
	else if(buf.st_mode & S_IFBLK)
		cout << 'b';
	else if(buf.st_mode & S_IFIFO)
		cout << 'f';
	else if(buf.st_mode & S_IFCHR)
		cout << 'c';
	else
		cout << '-';
	//user
	if(buf.st_mode & S_IRUSR)
		cout << 'r';
	else
		cout << '-';
	if(buf.st_mode & S_IWUSR)
		cout << 'w';
	else
		cout << '-';
	if(buf.st_mode & S_IXUSR)
		cout << 'x';
	else
		cout << '-';
	//group
	if(buf.st_mode & S_IRGRP)
		cout << 'r';
	else
		cout << '-';
	if(buf.st_mode & S_IWGRP)
		cout << 'w';
	else
		cout << '-';
	if(buf.st_mode & S_IXGRP)
		cout << 'x';
	else
		cout << '-';
	//other
	if(buf.st_mode & S_IROTH)
		cout << 'r'; 
	else
		cout << '-';
	if(buf.st_mode & S_IWOTH)
		cout<<'w';
	else
		cout << '-';
	if(buf.st_mode & S_IXOTH) 
		cout << 'x';
	else
		cout << '-';
	
}

void printcolor(struct stat buf, dirent *direntp)
{
		if(buf.st_mode & S_IFDIR)
			cout << "\033[38;5;39m" << direntp->d_name << "\033[0;00m" << " ";
		else if(buf.st_mode & S_IXUSR)
			cout << "\033[38;5;34m" << direntp->d_name << "\033[0;00m" << " ";
		else
			cout << direntp->d_name << " ";
}
void lflag(string dirName,bool af)
{
	if((strcmp(dirName.c_str(),"")) == 0)
	{
		dirName = ".";
	}
	DIR *dirp = opendir(dirName.c_str());
	if(errno!='\0')
	{
		perror("opendir");
		exit(1);
	}
	dirent *direntp;
	 while ((direntp = readdir(dirp)))
	 {
		 if(errno!='\0')
		 {
			 perror("readdir");
			 exit(1);
		 }
		struct stat st;
		char *path = new char[dirName.length()+150];

		strcpy(path,dirName.c_str());
		strcat(path, "/");
		strcat(path, direntp->d_name);
		if(stat(path, &st) == -1)
		{
			perror("stat");
			exit(1);
		}
		if(!af&&direntp->d_name[0] == '.')
			continue;
		printpermissions(st);
		cout << st.st_nlink << " ";
		struct passwd *id;
		string user;
		if(!(id = getpwuid(st.st_uid)))
		{
			perror("getpwuid");
			exit(1);
		}
		

		user = id->pw_name;
		cout << user << " ";
		struct group *gid;
		string getID;
		if(!(gid = getgrgid(st.st_gid)))
		{
			perror("getgrid");
			exit(1);
		}
		getID = gid->gr_name;
		cout << getID << " ";
			
		int bytesize = st.st_size;
		cout << bytesize << " ";
		time_t t = st.st_mtime;
		struct tm *time= localtime(&t);
		char timebuffer[100];
		strftime(timebuffer, 100, "%h %d %R", time);
		cout << timebuffer  << " ";
		printcolor(st,direntp);
	}
	cout<<endl;	
	closedir(dirp);
	if(errno==-1)
	{
		 perror("closedir");
		 exit(1);
	}
	return;
}
int rflag(string dirName,bool lf,bool af)
{
	vector<char*>directory;
	if((strcmp(dirName.c_str(),"")) == 0)
	{
		dirName = ".";
	}
	DIR *dirp;
	dirent *direntp;
	if(!(dirp = opendir(dirName.c_str())))
	{
		perror("opendir");
		exit(1);
	}
	if((strcmp(dirName.c_str(),".")==0))
		cout << ".:" << endl;
	else
		cout << dirName  <<":" << endl;
	while((direntp = readdir(dirp)))
	{
		if(errno != 0)
		{
			perror("readdir");
			exit(1);
		}
		struct stat st;
		char *path = new char[dirName.length()+150];
		strcpy(path,dirName.c_str());
		strcat(path,"/");
		strcat(path, direntp->d_name);
		if(stat(path, &st) == -1)
		{
			perror("stat");
			exit(1);
		}
		if(!af&&direntp->d_name[0]=='.')
			continue;
		else if(!lf)
			cout<<direntp->d_name<<" ";
		else
			lflag(dirName,af);

		if(S_ISDIR(st.st_mode))
			directory.push_back(direntp->d_name);
	}
	cout<<endl;
		for(unsigned p = 0; p < directory.size();p++)
			rflag(dirName + "/" + directory.at(p),lf,af);
		cout << endl;
		if(closedir(dirp) == -1)
		{
			perror("closedir");
			exit(1);
		}
	
		return 0;
}

int main(int argc, char* argv[])
{
	bool noflag=true;
	bool aflg=false;
	bool lflg=false;
	bool rflg=false;
	vector<char*> flags;
	for(int i=1;i<argc;++i)
	{
		flags.push_back(argv[i]);
	}
	if(argc>1)
		noflag=false;
	if(!noflag)
	{

		for(unsigned i=0; i<flags.size();++i)
		{
			if(strcmp(flags.at(i),"-a")==0)
				aflg=true;
			else if(strcmp(flags.at(i),"-l")==0)
				lflg=true;
			else if(strcmp(flags.at(i),"-R")==0)
				rflg=true;
			else if(strcmp(flags.at(i),"-la")==0||strcmp(flags.at(i),"-al")==0)
			{
				lflg=true;
				aflg=true;
			}
			else if(strcmp(flags.at(i),"-lR")==0||strcmp(flags.at(i),"-Rl")==0)
			{
				lflg=true;
				rflg=true;
			}
			else if(strcmp(flags.at(i),"-alR")==0||strcmp(flags.at(i),"-laR")==0
				||strcmp(flags.at(i),"-lRa")==0||strcmp(flags.at(i),"-Rla")==0
				||strcmp(flags.at(i),"-Ral")==0||strcmp(flags.at(i),"-aRl")==0)
			{
				lflg=true;
				rflg=true;
				aflg=true;
			}
		}
	}
	if(noflag)
		noflags();

	else if(aflg&&!rflg&&!lflg)//only -a
		aflag();

	else if(lflg&&!rflg&&!aflg)//only -l
		lflag("",aflg);
	else if(rflg&&!lflg&&!aflg)//only -R
		rflag("",lflg,aflg);
	else if(aflg&&lflg&&!rflg)//-a and -l
		lflag("",aflg);
	else if(rflg&&lflg&&!aflg)//-l and -R
		rflag("",lflg,aflg);
	//else if(rflg&&aflg&&!lflg)//-a and -R
	//rflag("",lflg,aflg);
	//else			//all flags
	//	rflag("",lflg,aflg);
	return 0;
}
