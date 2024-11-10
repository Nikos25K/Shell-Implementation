#include "includes.h"

//checks for wildcards and expands them
int check_wildcards(char* argv[], int* j, char* token){
    int i=0;
    bool wildcard = false;
    while(i < strlen(token))
        if(token[i] == '*' || token[i] == '?'||
        token[i] == '[' || token[i] == ']'){
            wildcard = true;
            break;
        }
        else
            i++;

    if(wildcard){
        glob_t*  globbuf = malloc(sizeof(glob_t));

        // Perform globbing
        int result = glob(token, 0, NULL, globbuf);
        if (result != 0){
            if (result == GLOB_NOMATCH)
                printf("No files matching pattern: %s\n", token);
            else
                perror("glob");
            globfree(globbuf);
            //error, so give another command
            return CONTINUE;
        }
        // Print matching files
        for (size_t i = 0; i < globbuf->gl_pathc; i++){
            // Allocate memory for the token
            argv[*j] = malloc((strlen(globbuf->gl_pathv[i]) + 1));
            strcpy(argv[*j], globbuf->gl_pathv[i]);
            (*j)++;
        }

        globfree(globbuf);
        return 1;
    }
    return 0;
}