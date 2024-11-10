#ifndef ALIASES_H
#define ALIASES_H

//data structure for the hashtable
typedef struct Hashtable hashtable;

//hash function
unsigned int djb2_hash(const char *str);

hashtable* create_hashtable();
void  insert(hashtable *ht, char *key, char *value);
char* search(hashtable *ht, char *key);
void  resize(hashtable *ht);
void  destroy_hashtable(hashtable *ht);

void create_alias(char *args[], int *argCount, hashtable *aliases);
void destroy_alias(char *args[], int *argCount, hashtable *aliases);
void print_aliases(hashtable *aliases, int fdout);

#endif