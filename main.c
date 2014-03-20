#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>

#define COUNT 100

int ls(char* direct){
	DIR *dir;
	dir = opendir(direct);
    
	if(dir == NULL){
		perror("Error. Unable to access the directory\n");
		_exit(1);
	}
    
	struct  dirent *file;
    int k = 1;
	while ((file = readdir(dir)) != 0){
		if((strcmp(file->d_name, ".") && strcmp(file->d_name, ".."))){
            printf("%d %s\n", k, file->d_name);
            ++k;
        }
        
	}
    
	closedir(dir);
	return 0;
}


int main (int argc, char** argv) {
    
	if (argc == 2){
		ls(argv[1]);
	}
    
	_exit(0);
}
