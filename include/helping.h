#ifndef HELPING_H
#define HELPING_H

//signal handling functions in signals.c

//catches the interrupt signal and sends it to the process
void catchinterrupt(int signo);
//sets the struct sigaction for the signals
void set_signals(struct sigaction *act);


//useful functions in utils.c

//duplicates a string
char* strdup(char* str);
//checks if malloc was successful
void  check_malloc(void* ptr);
//checks if the command is only whitespaces
bool  check_whitespaces(char* command);
//counts the number of pipes in the command
int   count_pipes(char* args[], int* argCount);


//parsing in utils.c

//reads the command from the user or from the alias and splits it into arguments
int  read_args(char* args[], int* argCount, char* alias);
//splits the command into arguments based on current pipe position
void split_pipe_args(char* args[], int* argCount, char* pipe_args[], int* pipe_argCount, int pipe_index);
//deletes the arguments
void delete_args(char* args[], int* argCount);

#endif