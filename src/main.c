#include "includes.h"

pid_t current_pid = -1;

int main() {
    // set signals
    static struct sigaction act;
    set_signals(&act);

    // create history and aliases data structures
    history* h = create_history();
    hashtable* aliases = create_hashtable();

    // to check if alias is used
    char* alias = NULL;

    while(1){
        // prompt
        char* args[MAX_ARGS];
        int argCount = 0;

        int check = read_args(args, &argCount, alias);
        //reset alias
        alias = NULL;

        // check if the user wants to exit or if the input is invalid
        if(check == EXIT){
            delete_args(args, &argCount);
            break;
        }
        else if(check == CONTINUE)
            continue;

        // if there is an alias, replace in args
        alias = search(aliases, args[0]);
        if(alias)
            continue;

        // check first if it is a simple command
        if(!execute_custom(args, &argCount, h, aliases))
            execute_pipeline(args, &argCount, h, aliases);
    }

    // free memory
    free_history(h);
    destroy_hashtable(aliases);
    return 0;
}