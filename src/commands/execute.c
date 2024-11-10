#include "includes.h"

extern pid_t current_pid;

bool execute_custom(char* args[], int* argCount, history* h, hashtable *ht){

    if(strcmp(args[0], "cd") == 0){
        console_filesystem(args, argCount);
        return true;
    }
    else if(strcmp(args[0], "ver") == 0){
        show_version(STDOUT_FILENO);
        return true;
    }
    else if(strcmp(args[0], "createalias") == 0){
        create_alias(args, argCount, ht);
        return true;
    }
    else if(strcmp(args[0], "destroyalias") == 0){
        destroy_alias(args, argCount, ht);
        return true;
    }
    else if(strcmp(args[0], "aliases") == 0){
        print_aliases(ht, STDOUT_FILENO);
        return true;
    }
    else if (strcmp(args[0], "history") == 0){
        //true if failed in order to give next command
        return manage_history(args, argCount, h);
    }
    return false;
}

void execute_command(char* args[], int* argCount, history* h, hashtable *ht,
                     int* fdout, int* fdin){

    bool waiting_child = true;

    if(args[*argCount-1][strlen(args[*argCount-1])-1] == '&'){
        args[*argCount-1] = NULL;
        waiting_child = false;
    }

    current_pid = fork();
    if(current_pid == -1){
        perror("fork");
        exit(ERROR);
    }
    else if(current_pid == 0){
        //file descriptors for redirection
        int fd_to = STDOUT_FILENO, fd_from = STDIN_FILENO;
        //files to write and read from
        int to = -1, from = -1;
        redirection(args, argCount, &fd_to, &fd_from, &to, &from);
        //set last argument to NULL in order to run execvp
        if(to > 0)
            args[to-1] = NULL;
        if(from > 0)
            args[from-1] = NULL;

        if(execvp(args[0], args) == -1){
            perror("execvp");
            exit(ERROR);
        }
    }
    else{
        int status;
        //parent waits and checks if the command should be added to history
        if(waiting_child)
            waitpid(current_pid, &status, 0);
    }

}

// helping function that sets the file descriptors for the pipe
// and calls the function to execute the command
void helper(char* args[], int* argCount, history* h, hashtable *ht, int in, int out){
  pid_t pid = fork();

    if (pid == -1){
        perror("fork");
        exit(ERROR);
    }
    //child sets and executes the command
    else if (pid == 0){
      if (in != 0){
          dup2 (in, 0);
          close (in);
        }

      if (out != 1){
          dup2 (out, 1);
          close (out);
        }

        execute_command(args, argCount, h, ht, &out, &in);
        exit(0);
    }
    //parent waits for the child and adds the command to history
    else{
        if(waitpid(pid, NULL, 0) == -1){
            perror("waitpid");
            exit(ERROR);
        }
        add_history(h, args);
    }
}

void execute_pipeline(char* args[], int* argCount, history* h, hashtable *ht) {

    int pipe_count = count_pipes(args, argCount);

    int pipefd[2];
    // first process from stdin
    int in = STDIN_FILENO;
    int out = STDOUT_FILENO;


    int i;  // loop variable for counting instructions between pipes

    char* pipe_args[MAX_ARGS];
    int pipe_argCount;
    for (i = 0; i < pipe_count; i++) {
        // split the args into pipe_args
        pipe_argCount = 0;
        split_pipe_args(args, argCount, pipe_args, &pipe_argCount, i);

        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(ERROR);
        }


        // pipefd[1] is the write end of the pipe
        // we carry in from the prev iteration
        helper(pipe_args, &pipe_argCount, h, ht, in, pipefd[1]);

        // No need for the write end of the pipe, the child will write here
        close(pipefd[1]);

        // Keep the read end of the pipe, the next child will read from there
        in = pipefd[0];
    }

    /* Execute the last stage with the current process. */
    pipe_argCount = 0;
    split_pipe_args(args, argCount, pipe_args, &pipe_argCount, i);

    helper(pipe_args, &pipe_argCount, h, ht, in, out);
}