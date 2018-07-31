/* * * * * * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * available hash functions:
 * method | description
 * --------------------
 *   'o'  | hash everything to zero
 *   'r'  | hash to a random value
 *   'a'  | hash to ascii value of first character (mod table size)
 *   'l'  | hash to length (mod table size)
 *   'p'  | build a hash value from the bytes of the first few characters
 *   'x'  | near-universal hash function based on xoring and bit shifting
 *   'u'  | universal hash function based on a random array
 * (add any new hash functions here)
 */


// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size);

// return the string name of a hash function specified by method
char *name(char method);
