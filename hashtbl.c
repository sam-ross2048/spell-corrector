/* * * * * * *
 * Hash table with separate chaining in a linked list
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * move-to-front added by ...
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hashtbl.h"

#include <string.h>
#include "strhash.h"


#define PRINT_LIMIT 10


/* * *
 * HELPER DATA STRUCTURE: LINKED LIST OF BUCKETS
 */

typedef struct bucket Bucket;
struct bucket {
	char *key;
	int value;
	Bucket *next;
};

Bucket *new_bucket(char *key, int value) {
	Bucket *bucket = malloc(sizeof *bucket);
	assert(bucket);

	// create own copy of key for storage in table
	bucket->key = malloc((sizeof *bucket->key) * (strlen(key) + 1));
	assert(bucket->key);
	strcpy(bucket->key, key);

	bucket->value = value;
	bucket->next = NULL;

	return bucket;
}

// Warning: does not free bucket->next
void free_bucket(Bucket *bucket) {
	assert(bucket != NULL);
	free(bucket->key);
	free(bucket);
}

struct table {
	int size;
	Bucket **buckets;
};


/* * *
 * HASH TABLE CREATION/DELETION
 */

HashTable *new_hash_table(int size) {
	HashTable *table = malloc(sizeof *table);
	assert(table);

	table->size = size;
	table->buckets = malloc(size * (sizeof *table->buckets));
	assert(table->buckets);
	int i;
	for (i = 0; i < size; i++) {
		table->buckets[i] = NULL;
	}

	return table;
}

void free_hash_table(HashTable *table) {
	assert(table != NULL);

	int i;
	for (i = 0; i < table->size; i++) {
		Bucket *this_bucket, *next_bucket;
		this_bucket = table->buckets[i];
		while (this_bucket) {
			next_bucket = this_bucket->next;
			free_bucket(this_bucket);
			this_bucket = next_bucket;
		}
	}

	free(table);
}


/* * *
 * HASHING HELPER FUNCTIONS
 */

int h(char *key, int size) {
	return hash(key, size);
}
bool equal(char *a, char *b) {
	return strcmp(a, b) == 0;
}


/* * *
 * HASH TABLE FUNCTIONS
 */

void hash_table_put(HashTable *table, char *key, int value) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	Bucket *prev = NULL;
	while (bucket) {
		if (equal(key, bucket->key)) {
			bucket->value = value;
			return;
		}
		prev = bucket;
		bucket = bucket->next;
	}

	// if key wasn't found, add it at front of list
	Bucket *new = new_bucket(key, value);
	new->next = table->buckets[hash_value];
	table->buckets[hash_value] = new;
}

int hash_table_get(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	while (bucket) {
		if (equal(key, bucket->key)) {
			return bucket->value;
		}
		bucket = bucket->next;
	}

	return 0;
}

bool hash_table_has(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = h(key, table->size);

	// look for existing key
	Bucket *bucket = table->buckets[hash_value];
	while (bucket) {
		if (equal(key, bucket->key)) {
			return true;
		}
		bucket = bucket->next;
	}

	// key doesn't exist!
	return false;
}


/* * *
 * PRINTING FUNCTIONS
 */

void print_hash_table(HashTable *table) {
	assert(table != NULL);
	fprint_hash_table(stdout, table);
}

void fprint_hash_table(FILE *file, HashTable *table) {
	assert(table != NULL);

	// max width of a table row label
	int width = snprintf(NULL, 0, "%d", table->size-1);

	int i, j;
	for (i = 0; i < table->size; i++) {
		fprintf(file, " %*d|", width, i);

		j = 0;
		Bucket *bucket;
		bucket = table->buckets[i];
		while (bucket && j < PRINT_LIMIT) {
			fprintf(file, "->(\"%s\": %d)", bucket->key, bucket->value);
			bucket = bucket->next;
			j++;
		}
		if (bucket) {
			fprintf(file, "...");
		}
		fprintf(file, "\n");
	}
}
