#include "includes.h"

struct Node{
    char data[MAX_CHARS];
    struct Node* next;
};

struct History{
    struct Node* head;
    struct Node* tail;
    int size;
};
typedef struct Node node;


history* create_history(){
    history* h = (history*)malloc(sizeof(history));
    h->head = NULL;
    h->tail = NULL;
    h->size = 0;
    return h;
}


void free_node(node* n){
    free(n);
}

void free_history(history* h){
    node* temp = h->head;
    while(temp != NULL){
        node* next = temp->next;
        free(temp);
        temp = next;
    }
    free(h);
}


void add_history(history* h, char* args[]){       //will add in start of the list
    node* n = (node*)malloc(sizeof(node));
    check_malloc(n);

    int i = 0;
    //because strcat appends to the end of the string
    n->data[0] = '\0';
    while(args[i] != NULL){
        strcat(n->data, args[i++]);
        strcat(n->data, " ");
    }
    n->next = h->head;
    h->head = n;

    if(h->size == MAX_HISTORY -1){
        node* temp = h->head;
        while (temp->next != h->tail)
            temp = temp->next;
        free_node(h->tail);
        temp->next = NULL;
        h->tail = temp;
    }
    else
        h->size++;

}

void print_history(history* h){
    node* temp = h->head;
    int i = 0;
    while(temp != NULL){
        printf("%d. %s\n", i, temp->data);
        temp = temp->next;
        i++;
    }
}

//if one argument is given, it will print the history
//if two arguments are given, it will execute the number of second argument from the history
//returns true if the command is not executed
bool manage_history(char* args[], int* argCount, history* h){
    //no args
    if(!args[1]){
        print_history(h);
        return true;
    }

    //check if the second argument is a number
    if (!isdigit(args[1][0])) {
        printf("Usage: history <number>\n");
        return true;
    }

    //convert the second argument to integer
    //and check if it is in the range
    int n = atoi(args[1]);
    if(*argCount > 2 || n < 0 || n > h->size){
        printf("Usage: history <number>\n");
        return true;
    }

    node* temp = h->head;
    int i = 0;
    while(i < n){
        temp = temp->next;
        i++;
    }

    // Tokenize temp->data without modifying it
    char* temp_data_copy = strdup(temp->data);

    i = 0;
    char* token = strtok(temp_data_copy, " ");
    while (token != NULL) {
        args[i] = malloc((strlen(token) + 1) * sizeof(char));
        check_malloc(args[i]);

        strcpy(args[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    *argCount = i;

    // Free temporary memory
    free(temp_data_copy);

    return false;
}