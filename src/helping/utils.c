#include "includes.h"

//duplicates a string
char* strdup(char* str){
    char* new_key = malloc(strlen(str) + 1);
    check_malloc(new_key);
    strcpy(new_key, str);
    return new_key;
}

//checks if malloc was successful
void check_malloc(void* ptr){
    if(!ptr){
        fprintf(stderr,"Memory allocation failed\n");
        exit(1);
    }
}

// Check if all characters in the command are whitespaces
bool check_whitespaces(char* command){
    int i;
    for (i = 0; i < strlen(command); i++)
        if (!isspace(command[i]))
            break;

    return (i == strlen(command)) ? true : false;
}

//counts the number of pipes in the command
int count_pipes(char* args[], int* argCount){
    int count = 0;
    for (int i = 0; i < *argCount; i++)
        if (strcmp(args[i], "|") == 0)
            count++;
    return count;
}

//reads the command from the user or from the alias and splits it into arguments
int read_args(char* args[], int* argCount, char* alias) {
    char *command = NULL;

    if(!alias){
        printf("in-mysh-now:> ");
        command = malloc(MAX_CHARS);
        check_malloc(command);
        // Read the command from the user or if alias from file
        fgets(command, MAX_CHARS, stdin);
    }
    else{
        command = malloc(strlen(alias) - 2 - 2 + 1);
        check_malloc(command);
        strncpy(command, alias + 2, strlen(alias) - 2 - 2);
        command[strlen(alias) - 2 - 2] = '\0';
    }

    //Check if there are only whitespaces
    if(check_whitespaces(command))
        return CONTINUE;

    // Tokenize the command into separate words
    char* token = strtok(command, " \n");

    // Maximum 10 arguments
    while (token != NULL && *argCount < MAX_ARGS) {
        // Check for wildcards (result saved in args array)
        int check = 0;
        // so that createalias can have wildcards (and not be expanded)
        if(*argCount>0 && strcmp(args[0], "createalias") == 0)
            check=0;
        else{
            check = check_wildcards(args, argCount, token);
            if(check == CONTINUE)
                return CONTINUE;
        }

        // If there are no wildcards, then add the token to the arguments
        if(!check){
            // Allocate memory for the token
            args[*argCount] = malloc((strlen(token) + 1) * sizeof(char));
            check_malloc(args[*argCount]);

            // Copy the token to the dynamically allocated memory
            strcpy(args[*argCount], token);
            (*argCount)++;
        }
        // Get the next token
        token = strtok(NULL, " \n");
    }
    // Last element should be NULL
    args[*argCount] = NULL;
    free(command);

    if(strcmp(args[0], "exit") == 0)
        return EXIT;
    return 0;
}

// from args, keep adding to pipe_args until you reach the pipe symbol(pipe index)
// update pipe_argCount and pipe_args
void split_pipe_args(char* args[], int* argCount, char* pipe_args[], int* pipe_argCount, int pipe_index){
    int i = 0;
    int pipe_count = 0;

    // find the index of the pipe symbol, so after the for loop,
    // i will be the position in args after the desired | symbol
    for(i=0; i < *argCount; i++){
        if(pipe_count == pipe_index)
            break;
        if(strcmp(args[i], "|") == 0)
            pipe_count++;
    }

    for(int j=i; j < *argCount; j++){
        // if pipe symbol is found or end of args is reached, break
        if(strcmp(args[j], "|") == 0)
            break;
        else{
            pipe_args[*pipe_argCount] = args[j];
            (*pipe_argCount)++;
        }
    }
    // add NULL to the end of pipe_args
    pipe_args[*pipe_argCount] = NULL;
}

//deletes the arguments
void delete_args(char* args[], int* argCount){
    for(int i=0; i<*argCount; i++)
        free(args[i]);
    *argCount = 0;
}