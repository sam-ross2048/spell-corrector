/* * * * * * *
 * Hash table with separate chaining in a linked list
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * move-to-front added by ...
 */

#include <stdbool.h>

typedef struct table HashTable;

HashTable *new_hash_table(int size);
void free_hash_table(HashTable *table);

void hash_table_put(HashTable *table, char *key, int value);
int  hash_table_get(HashTable *table, char *key);
bool hash_table_has(HashTable *table, char *key);

void print_hash_table(HashTable *table);
void fprint_hash_table(FILE *file, HashTable *table);