#include "includes.h"
#define INIT_SIZE 20

#define MAX_LOAD_FACTOR 0.5


struct Entry {
    char *key;
    char *value;
};

typedef struct Entry entry;

struct Hashtable {
    entry *entries;
    int size;
    int capacity;
};

//Hash function
unsigned int djb2_hash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

hashtable *create_hashtable() {
    hashtable *ht = malloc(sizeof(hashtable));
    ht->size = 0;
    ht->capacity = INIT_SIZE;
    ht->entries = malloc(sizeof(entry) * ht->capacity);

    for (int i = 0; i < ht->capacity; i++) {
        ht->entries[i].key = NULL;
        ht->entries[i].value = NULL;
    }

	char buf[MAX_CHARS];
	snprintf(buf, sizeof(buf), "%s/.aliases", getenv("HOME"));
    FILE *fptr = fopen(buf, "r");

	if(!fptr)   //file does not exist
		return ht;

    int entries;
	if(fscanf(fptr, "%d ", &entries) > 0){
		for(int i = 0; i < entries; i++){
			char *key = malloc(MAX_CHARS);
			char *value = malloc(MAX_CHARS);
			char c;
			int j = 0;
			while((c = fgetc(fptr)) != '\n')
				key[j++] = c;
			key[j] = '\0';
			j = 0;
			while((c = fgetc(fptr)) != '\n'){
				value[j++] = c;
			}
			value[j] = '\0';
			insert(ht, key, value);
			free(key);
			free(value);
		}
	}
	fclose(fptr);

    return ht;
}

void destroy_hashtable(hashtable *ht) {

    //first save to file
	char buf[MAX_CHARS];
	snprintf(buf, sizeof(buf), "%s/.aliases", getenv("HOME"));
	FILE *fptr = fopen(buf, "w");
	if(!fptr){
		fprintf(stderr, "Error opening aliases file\n");
		exit(ERROR);
	}

	fprintf(fptr, "%d\n", ht->size);
	for(int i = 0; i < ht->capacity; i++)
        if(ht->entries[i].key != NULL){
            fprintf(fptr, "%s\n", ht->entries[i].key);
            fprintf(fptr, "%s\n", ht->entries[i].value);
        }

	fclose(fptr);

    //now delete them
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->entries[i].key != NULL) {
            free(ht->entries[i].key);
            free(ht->entries[i].value);
        }
    }

    free(ht->entries);
    free(ht);
}

void resize(hashtable *ht){
    int old_capacity = ht->capacity;
    entry *old_entries = ht->entries;

    ht->capacity *= 2;
    ht->entries = malloc(sizeof(entry) * ht->capacity);

    for (int i = 0; i < ht->capacity; i++) {
        ht->entries[i].key = NULL;
        ht->entries[i].value = NULL;
    }

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].key != NULL) {
            insert(ht, old_entries[i].key, old_entries[i].value);
            free(old_entries[i].key);
            free(old_entries[i].value);
        }
    }

    free(old_entries);
}

void insert(hashtable *ht, char *key, char *value){
    unsigned int index = djb2_hash(key) % ht->capacity;

    //collision
    while (ht->entries[index].key != NULL){
        //if key already exists, update value
        if (strcmp(ht->entries[index].key, key) == 0) {
            free(ht->entries[index].value);
            ht->entries[index].value = strdup(value);
            return;
        }
        index = (index + 1) % ht->capacity;
    }

    ht->entries[index].key = strdup(key);
    ht->entries[index].value = strdup(value);

    ht->size++;
    if ((double) ht->size / ht->capacity > MAX_LOAD_FACTOR)
        resize(ht);
}

void delete(hashtable *ht, char *key){
    unsigned int index = djb2_hash(key) % ht->capacity;

    while (ht->entries[index].key != NULL) {
        if (strcmp(ht->entries[index].key, key) == 0) {
            free(ht->entries[index].key);
            free(ht->entries[index].value);
            ht->entries[index].key = NULL;
            ht->entries[index].value = NULL;
            ht->size--;
            return;
        }

        index = (index + 1) % ht->capacity;
    }
}

char* search(hashtable *ht, char *key){
    unsigned int index = djb2_hash(key) % ht->capacity;

    while (ht->entries[index].key != NULL) {
        if (strcmp(ht->entries[index].key, key) == 0)
            return ht->entries[index].value;

        index = (index + 1) % ht->capacity;
    }
    return NULL;
}


void create_alias(char *args[], int *argCount, hashtable *aliases){
    if (args[2][0] != '"' || args[*argCount-1][strlen(args[*argCount-1])-2] !='"'
    || args[*argCount-1][strlen(args[*argCount-1])-1] != ';'){
        fprintf(stderr,"Usage: createalias <alias> \"command\";\n");
        return;
    }
    if (isdigit(args[1][0]) || isdigit(args[2][0])) {
        fprintf(stderr,"Usage: createalias <alias> \"command\";\n");
        return;
    }
    if (strcmp(args[1], "history") == 0) {
        fprintf(stderr,"Cannot create alias for history\n");
        return;
    }

    // Concatenate all arguments starting from args[2]
    int len = 0;
    for (int i = 2; i < *argCount; i++)
        len += strlen(args[i]);

    // +1 for '\0' and + *argCount-2 for spaces
    char *command = malloc(len + 1 + *argCount-2);

    //because strcat appends to the end of the string
    command[0] = '\0';

    for (int i = 2; i < *argCount; i++) {
        strcat(command, " ");
        strcat(command, args[i]);
    }

    args[2] = command;
    for (int i = 3; i < *argCount; i++) {
        free(args[i]);
        args[i] = NULL;
    }
    *argCount = 3;

    insert(aliases, args[1], args[2]);
}

void destroy_alias(char *args[], int *argCount, hashtable *aliases){
    if (*argCount != 2){
        fprintf(stderr, "Usage: destroyalias <alias>\n");
        return;
    }
    if (isdigit(args[1][0])){
        fprintf(stderr, "Usage: destroyalias <alias>\n");
        return;
    }

    delete(aliases, args[1]);
}

void print_aliases(hashtable *aliases, int fdout){
    int count = 0;
    char buffer[MAX_CHARS];
    for (int i = 0; i < aliases->capacity; i++) {
        if (aliases->entries[i].key != NULL) {
            snprintf(buffer, sizeof(buffer), "%d. %s -> %s\n", count, aliases->entries[i].key, aliases->entries[i].value);
            write(fdout, buffer, strlen(buffer));
            count++;
        }
    }
}