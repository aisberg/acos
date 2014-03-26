#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>

#define BUF 100

int lflag = 0, rflag = 0;
void printLs(char *path);

void checkFlags(int argc, char *argv[])
{
    int ch;
    while ((ch = getopt(argc,argv,"lR")) != -1)
    {
        switch (ch)
        {
            case 'l':
                lflag = 1;
                break;
            case 'R':
                rflag= 1;
                break;
            default:
                printf("this argument is incorect\n");
                exit(1);
                
        }
    }
}

int ls(char* direct){
	DIR *dir;
	dir = opendir(direct);
    
	if(!dir){
		perror("Error. Unable to access the directory\n");
		_exit(1);
	}
    
	struct  dirent *file;
    
	while ((file = readdir(dir)) != 0){
		if(file->d_name[0] == '.') continue;
        
        printLs(file->d_name);
        
        
	}
    
	closedir(dir);
	return 0;
}

int rls(char* direct){
    
	DIR *dir;
    struct stat inform;
	dir = opendir(direct);
    
	if(!dir){
		perror("Error. Unable to access the directory\n");
		_exit(1);
	}
    
	struct  dirent *file;
    char newpath[PATH_MAX + 1];
    
	while ((file = readdir(dir)) != 0){
		if(file->d_name[0] == '.') continue;
        
        strncpy( newpath, direct, PATH_MAX );
        strncat( newpath, "/", PATH_MAX );
        strncat( newpath, file->d_name, PATH_MAX );
        
        if (stat(newpath, &inform ) < 0)
        {
            fprintf (stderr, "ls: Cannot open file\n");
            exit(1);
        }
        
        
        printLs(newpath);
        
		if (inform.st_mode & S_IFDIR){
			rls(newpath);
        }
        
        
        
        
	}
    
	closedir(dir);
	return 0;
}

void printLs(char *path)
{
    struct stat inform;

    struct passwd *uid;
    
    if (stat(path, &inform) < 0)
    {
        printf("Cannot open file or file does not exist.\n");
        exit(2);
    }

    if (lflag == 1)
    {
        (S_ISDIR(inform.st_mode)) ? printf("d:"): (S_ISLNK(inform.st_mode)) ? printf("l:"): printf("-");
        (inform.st_mode & S_IRUSR) ? printf("r"): printf("-");
        (inform.st_mode & S_IWUSR) ? printf("w"): printf("-");
        (inform.st_mode & S_IXUSR) ? printf("x"): printf("-");
        (inform.st_mode & S_IRGRP) ? printf("r"): printf("-");
        (inform.st_mode & S_IWGRP) ? printf("w"): printf("-");
        (inform.st_mode & S_IXGRP) ? printf("x"): printf("-");
        (inform.st_mode & S_IROTH) ? printf("r"): printf("-");
        (inform.st_mode & S_IWOTH) ? printf("w"): printf("-");
        (inform.st_mode & S_IXOTH) ? printf("x"): printf("-");
        
        if ((uid = getpwuid(inform.st_uid))==NULL)
            printf("Can't identify the user name");
        
        if ((uid = getpwuid(inform.st_uid)) != NULL)
            printf(" %s  ",uid->pw_name);
        
        if ((uid = getpwuid(inform.st_uid)) != NULL)
            printf("%d  ",uid->pw_gid);
        else
            printf("Can't identify the id group");
        
        printf("%10ld ", inform.st_size);

        printf("%s\n", path);
    }
    else
    {
        printf("%s\n", path);
    }
}

int main (int argc, char** argv) {
    
    checkFlags(argc,argv);
    
    if(argc == 1){
        char dirName[PATH_MAX + 1];
        
        getcwd(dirName, PATH_MAX);
        ls(dirName);
    }
    
    if(argc == 2){
        if((lflag && !rflag) || (!lflag && rflag)){
            char dirName[PATH_MAX + 1];
    
            getcwd(dirName, PATH_MAX);
            
            if(lflag && !rflag)
               ls(dirName);
            else rls(dirName);
        }
        if(!lflag && !rflag)
            ls(argv[1]);
    }
    
    if(argc == 3){
        char dirName[PATH_MAX + 1];
        getcwd(dirName, PATH_MAX);
        
        if(lflag && rflag){
            
            rls(dirName);
        }
        if((lflag && !rflag) || (!lflag && rflag))
           if(lflag && !rflag)
              ls(dirName);
           else rls(dirName);
    }
    
    if(argc == 4){
        rls(argv[1]);
    }

    
	_exit(0);
}
