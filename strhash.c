/* * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "strhash.h"

#define MAX_KEY_LEN 256
#define SIZE 100000



// random-array based universal hash function
// uses a static flag to initialise the random array the first time the function
// is called, and uses the same array afterwards
// assumes strlen(key) <= MAX_KEY_LEN = 128
unsigned int universal_hash(const char *key, unsigned int size) {
	static int *r = NULL;
	if (!r) {
		r = malloc(MAX_KEY_LEN * (sizeof *r));
		assert(r);

		int i;
		for (i = 0; i < MAX_KEY_LEN; i++) {
			r[i] = rand();
		}
	}

	unsigned int h = 0;
	int i;
	for (i = 0; key[i] != '\0'; i++) {
		h = h + r[i] * key[i];
	}

	return h % size;
}




// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size) {
	return universal_hash(key, size);
}
