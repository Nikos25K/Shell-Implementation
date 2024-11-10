#ifndef COMMANDS_H
#define COMMANDS_H

//execute.c

//execute simple commands
bool execute_custom(char* args[], int* argCount, history* h, hashtable *ht);

//execute shell command with execvp
void execute_command(char* args[], int* argCount, history* h, hashtable *ht,
                     int* fdout, int* fdin);

//execute pipeline
void execute_pipeline(char* args[], int* argCount, history* h, hashtable *ht);                  


//wildcards.c

//checks for wildcards and expands them
int check_wildcards(char* argv[], int* j, char* token);


//redirect.c

//Redirects the input and output of the command to the files
void redirection(char* args[], int* argCount, int* fd_to, int* fd_from, int* to, int* from);
//checks for redirection and the type of it
int check_redirection(char* args[], int* argCount);


//simple_commands.c

//shows version of the shell
void show_version(int fdout);
//executes the cd command
bool console_filesystem(char* args[], int* argCount);

#endif