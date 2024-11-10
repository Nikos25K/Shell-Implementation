#ifndef HISTORY_H
#define HISTORY_H

//data structure for the list
typedef struct History history;

history* create_history();
void add_history(history* h, char* args[]);
void print_history(history* h);
void free_history(history* h);

//if one argument is given, it will print the history
//if two arguments are given, it will execute the number of second argument from the history
bool manage_history(char* args[], int* argCount, history* h);
#endif