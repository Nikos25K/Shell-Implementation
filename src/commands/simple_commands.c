#include "includes.h"

//shows version of the shell
void show_version(int fdout){
    char* show = "mySH version 1.0.0 \n";
    write(fdout, show, strlen(show));
}

//executes the cd command
bool console_filesystem(char* args[], int* argCount){
    if(*argCount != 2){
        fprintf(stderr, "Usage: cd <directory>\n");
        return false;
    }
    // Change the current directory(even if not, will be in history)
    if(chdir(args[1]) == -1){
        perror("chdir");
        return false;
    }

    char cwd[MAX_CHARS];

    if (getcwd(cwd, sizeof (cwd)))
        fprintf(stderr, "Current directory set to: %s\n", cwd);

    return true;
}