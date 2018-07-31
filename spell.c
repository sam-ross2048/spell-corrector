/* * * * * * *
 * Module for spell-checking and correction of documents using
 * Levenshtein edit-distance and edit generation.
 * Makes use of hash-table functions created by Matt Farrugia.
 * Created for Assignment 2 COMP20007 Design of Algorithms 2018 Semester One
 * by Sam Ross (Student No. 914474).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spell.h"
#include "list.h"
#include "hashtbl.h"
#include <time.h>

#define HASH_SIZE 1000000
#define MAX_WORD_LEN 256


typedef struct{
	char* word;
	int position;
} valid_word_t;


// Function calculates the minimum of three integer values.
int min(int substitution, int deletion, int insertion){
	int min_ = substitution;

	if(deletion<min_){
		min_ = deletion;
	}
	if(insertion<min_){
		min_ = insertion;
	}
	return min_;
}


// Function finds all words 1 edit-distance away from a word.
void find_all_edits(List* edits, char* word){
	char alphabet[] = "abcdefghijklmnopqrstuvwxyz\0";
	int n = strlen(word);
	int i, j;
	char* new_word = (char*)malloc(sizeof(char)*(n+1));
	strcpy(new_word, word);

	// Finds all deletions and adds to edits.
	for(i=0;i<n;i++){
		new_word = (char*)malloc(sizeof(char)*(n+1));
		strcpy(new_word, word);
		for(j=i;j<n;j++){
			new_word[j] = new_word[j+1];
		}
		list_add_end(edits, new_word);
	}

	// Finds all substitutions and adds to edits.
	for(i=0;i<n;i++){
		for(j=0;j<strlen(alphabet);j++){
			new_word = (char*)malloc(sizeof(char)*(n+1));
			strcpy(new_word, word);
			new_word[i] = alphabet[j];
			list_add_end(edits, new_word);
		}
	}

	// Finds all insertions and adds to edits.
	int z;
	for(i=0;i<=n;i++){
		for(j=0;j<strlen(alphabet);j++){
			new_word = (char*)malloc(sizeof(char)*(n+2 ));
			strcpy(new_word, word);
			for(z=n+1;z>i;z--){
				new_word[z] = new_word[z-1];
			}
			new_word[i] = alphabet[j];
			list_add_end(edits, new_word);
		}
	}
}


// Function returns the edit distance between two strings.
int find_edit_distance(char* word1, char* word2){
	int n = strlen(word1);
	int m = strlen(word2);

	/* Matrix where E[i, j] is the edit distance between first i letters
	   of word1 and the first j letters of word2. */
	int E[n+1][m+1];
	int i;
	// Fill in known values.
	for(i=0;i<=n;i++){
		E[i][0] = i;
	}
	for(i=1;i<=m;i++){
		E[0][i] = i;
	}
	int sub_cost;
	int j;
	// Fill in rest of values (adapted from pseudocode described on LMS).
	for(i=1;i<=n;i++){
		for(j=1;j<=m;j++){
			if(word1[i-1] == word2[j-1]){
				sub_cost = 0;
			}
			else{
				sub_cost = 1;
			}
			E[i][j] = min(E[i-1][j-1] + sub_cost, E[i-1][j] + 1, E[i][j-1] + 1);
		}
	}
	// Return final edit distance.
	return E[n][m];
}


// Function prints the edit distance between two words.
void print_edit_distance(char *word1, char *word2) {
	printf("%d\n", find_edit_distance(word1, word2));
}


// Function prints all strings 1 edit-distance away from a given word.
void print_all_edits(char *word) {

	// Creates linked list with all edits in it.
	List* edits = new_list();
	find_all_edits(edits, word);

	Node* current = edits->head;
	char* edit = current->data;
	while(current){
		edit = current->data;
		printf("%s\n", edit);
		free(current->data);
		current=current->next;
	}
	free_list(edits);
}


// Function puts contents of a linked list into a hash table.
HashTable* put_dict_into_hash(List *dictionary){
	HashTable* dict = new_hash_table(HASH_SIZE);
	// Value stored in hash table is the position within the linked list.
	int position = 1;
	Node* current = dictionary->head;
	while(current){
		hash_table_put(dict, current->data, position);
		position++;
		current=current->next;
	}
	return dict;
}


/* Given a list of words, prints whether or not the word is within the
   dictionary. */
void print_checked(List *dictionary, List *document) {
	// Puts dictionary into a hash table to make lookups much faster.
	HashTable* dict = put_dict_into_hash(dictionary);
	// Checks whether each word is in the dictionary.
	Node* current = document->head;
	char* word = current->data;
	while(current){
	 	word = current->data;
		if(hash_table_get(dict, current->data)){
			printf("%s\n", word);
		}
		else{
			printf("%s?\n", word);
		}
		current = current->next;
	}
}


/* Prints the valid word that is the best correction (based upon position
   within original dictionary linked list). */
void print_best_match(List* valid_words){
	Node* current = valid_words->head;
	valid_word_t* curr_data = current->data;
	int lowest_position = curr_data->position;
	char* lowest_word = curr_data->word;

	while(current){
		curr_data = current->data;
		if(curr_data->position < lowest_position){
			lowest_position = curr_data->position;
			lowest_word = curr_data->word;

		}
		current = current->next;
	}
	printf("%s\n", lowest_word);
}


/* Finds whether a word exists in dict that is 3 edits away
   from the original word. */
void third_edit_distance(List* dictionary, char* word){

	Node* current = dictionary->head;
	char* valid = current->data;
	// Calculates edit distance of each word to original word.
	while(current){
		valid = current->data;
		// If edit distance equals three, then that word can be printed.
		if(find_edit_distance(valid, word) <= 3){
			printf("%s\n", valid);
			return;
		}
		current = current->next;

	}
	// If no such word exists - print original word with '?'.
	printf("%s?\n", word);
}


// Finds whether a word exists in dict that is 2 edits away from original word.
void second_edit_distance(List* edits_queue, HashTable* edits_table,
					HashTable* dict, List* dict_list, char* word){

	List* final_edits = new_list();
	List* valid_words = new_list();

	/* Runs through list of edits, and for each edit, finds all the edits of
	   that edit, making sure not to do repeat computation. */
	Node* curr_edit = edits_queue->head;
	while(curr_edit){
		List* new_queue = new_list();
		// Finds all edits of current edit.
		find_all_edits(new_queue, curr_edit->data);
		// Adds new edits, to 'final_edits' list.
		Node* new_edit = new_queue->head;
		while(new_edit){
			// Only adds to 'final_edits' if not already in 'final_edits'.
			if(!(hash_table_get(edits_table, new_edit->data))){
				hash_table_put(edits_table, new_edit->data, 2);
				list_add_start(final_edits, new_edit->data);
			}
			new_edit = new_edit->next;
		}
		free_list(new_queue);
		curr_edit = curr_edit->next;
	}


	curr_edit = final_edits->head;
	while(curr_edit){
		// Position is the position of a valid word in the dictionary list.
		int position;
		// Finds whether any of the edits are in the dictionary.
		if((position = hash_table_get(dict, curr_edit->data))){
			valid_word_t* valid_word = malloc(sizeof(valid_word_t));
			valid_word->word = curr_edit->data;
			valid_word->position = position;
			// Adds valid word to 'valid_words' list, along with position.
			list_add_start(valid_words, valid_word);
		}
		curr_edit = curr_edit->next;
	}
	/* If there are no valid words, check whether any valid words exist 3
	   edits away from the word. */
	if(!(valid_words->head)){
		third_edit_distance(dict_list, word);
	}
	// Otherwise print valid word with lowest position in dictionary list.
	else{
		print_best_match(valid_words);
	}
}


// Finds whether a word exists in dict that is 1 edit away from original word.
void find_correct_word(HashTable* dict, List* dict_list, char* word){
	int edit_distance = 1;
	List* edits_queue = new_list();
	find_all_edits(edits_queue, word);
	List* valid_words = new_list();
	// Hash Table to contain all edits that have been seen.
	HashTable* edits_table = new_hash_table(HASH_SIZE);
	Node* edit_node = edits_queue->head;

	// Runs through edits of original word.
	while(edit_node){
		// Puts each edit into edits table.
		hash_table_put(edits_table, edit_node->data, edit_distance);
		int position;
		//  Checks whether edit is a valid word, if so inserts into linked list.
		if((position = hash_table_get(dict, edit_node->data))){
			valid_word_t* valid_word = malloc(sizeof(valid_word_t));
			valid_word->word = edit_node->data;
			valid_word->position = position;
			list_add_start(valid_words, valid_word);
		}
		edit_node = edit_node->next;
	}
	// If no valid words exist, check second edit distance.
	if(!(valid_words->head)){
		second_edit_distance(edits_queue, edits_table, dict, dict_list, word);
	}
	// Otherwise print best match based upon position.
	else{
		print_best_match(valid_words);
	}
	free_list(valid_words);
	free_hash_table(edits_table);
}


// Prints list of words, attempting to correct 'mispelt' words.
void print_corrected(List *dictionary, List *document) {
	// Converts dictionary list into a hash table.
	HashTable* dict = put_dict_into_hash(dictionary);

	// Checks whether each word is in dictionary or not.
	Node* current = document->head;
	char* word = current->data;
	while(current){
		word = current->data;
		if(hash_table_get(dict, current->data)){
			printf("%s\n", word);
		}
		// If not, attempts to find a correction.
		else{
			find_correct_word(dict, dictionary, current->data);
		}
		current = current->next;
	}
}
