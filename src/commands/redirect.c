#include "includes.h"

// Redirects the input and output of the command to the files
void redirection(char* args[], int* argCount, int* fd_to, int* fd_from, int* to, int* from){
    for(int i=0; i<*argCount; i++){
        if (strcmp(args[i], "<") == 0) {
            if (i < *argCount-1 && args[i+1] != NULL) {
                *fd_from = open(args[i+1], O_RDONLY);
                *from = i+1;  //so that we place NULL before it
                close(0);
                dup(*fd_from);
                close(*fd_from);
            }
            else
                perror("open failed");
        }
        else if (strcmp(args[i], ">") == 0) {
            if (i < *argCount-1 && args[i+1] != NULL) {
                *fd_to = open(args[i+1], O_CREAT | O_TRUNC | O_WRONLY, 0666);
                *to = i+1;  //so that we place NULL before it
                close(1);
                dup(*fd_to);
                close(*fd_to);
            }
            else
                perror("open failed");
        }
        else if (strcmp(args[i], ">>") == 0) {
            if (i < *argCount-1 && args[i+1] != NULL) {
                *fd_to = open(args[i+1], O_APPEND | O_WRONLY, 0666);
                *to = i+1;  //so that we place NULL before it
                close(1);
                dup(*fd_to);
                close(*fd_to);
            }
            else
                perror("open failed");
        }
    }
}

//checks for redirection and the type of it
int check_redirection(char* args[], int* argCount){

    // First and last argument cannot be redirection
    if(strcmp(args[0], ">") == 0 || strcmp(args[0], ">>") == 0 || strcmp(args[0], "<") == 0
    || strcmp(args[*argCount -1], ">") == 0 || strcmp(args[*argCount -1], ">>") == 0
    || strcmp(args[*argCount -1], "<") == 0)
        return ERROR;

    // Check for multiple redirections
    int redir_to = 0;
    int redir_from = 0;
    for(int i=1; i<*argCount; i++){
        if(strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
            redir_to++;
        else if(strcmp(args[i], "<") == 0)
            redir_from++;
    }
    if(redir_to > 1 || redir_from > 1)
        return ERROR;

    if(redir_to == 1 && redir_from == 0)
        return REDIR_TO;   // Output redirection
    else if(redir_to == 0 && redir_from == 1)
        return REDIR_FROM;   // Input redirection
    else if(redir_to == 1 && redir_from == 1)
        return BOTH;   // Both input and output redirection
    else
        return CONTINUE;   // No redirection
}
