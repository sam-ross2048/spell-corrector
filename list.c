/* * * * * * *
 * Module for creating and manipulating singly-linked lists containing data
 * of any type
 *
 * created for COMP20007 Design of Algorithms - Assignment 1, 2018
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdlib.h>
#include <assert.h>
#include "list.h"

/*                         DO NOT CHANGE THIS FILE
 * 
 * DO NOT add or modify any definitions or anything else inside this file.
 *
 * We will test your assignment with an unmodified version of this file. Any
 * changes you make will be lost. This may lead to compile errors.
 */


// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);

/* * * *
 * FUNCTION DEFINITIONS
 */

// create a new, empty list and return its pointer
List *new_list() {
	List *list = malloc(sizeof *list);
	assert(list);
	
	list->head = NULL;
	list->last = NULL;
	list->size = 0;

	return list;
}

// destroy a list and free its memory
// DOES NOT FREE POINTERS TO DATA HELD IN THE LISTS NODES, only frees the nodes
void free_list(List *list) {
	assert(list != NULL);
	// free each node
	Node *node = list->head;
	Node *next;
	while (node) {
		next = node->next;
		free_node(node);
		node = next;
	}
	// free the list struct itself
	free(list);
}

// helper function to create a new node and return its address
// DOES NOT INITIALISE THE NODE'S DATA
Node *new_node() {
	Node *node = malloc(sizeof *node);
	assert(node);
	
	return node;
}

// helper function to clear memory of a node
// DOES NOT FREE THE NEXT NODE OR THE NODE'S DATA
void free_node(Node *node) {
	free(node);
}

// add an element to the front of a list
// this operation is O(1)
void list_add_start(List *list, void *data) {
	assert(list != NULL);

	// create and initialise a new list node
	Node *node = new_node();
	node->data = data;
	node->next = list->head; // next will be the old first node (may be null)

	// place it at the start of the list
	list->head = node;

	// if list was empty, this new node is also the last node now
	if(list->size == 0) {
		list->last = node;
	}

	// and we need to keep size updated!
	list->size++;
}

// add an element to the back of a list
// this operation is O(1)
void list_add_end(List *list, void *data) {
	assert(list != NULL);
	
	// we'll need a new list node to store this data
	Node *node = new_node();
	node->data = data;
	node->next = NULL; // as the last node, there's no next node

	if(list->size == 0) {
		// if the list was empty, new node is now the first node
		list->head = node;
	} else {
		// otherwise, it goes after the current last node
		list->last->next = node;
	}
	
	// place this new node at the end of the list
	list->last = node;
	
	// and keep size updated too
	list->size++;
}

// remove and return the front data element from a list
// this operation is O(1)
// error if the list is empty (so first ensure list_size() > 0)
void *list_remove_start(List *list) {
	assert(list != NULL);
	assert(list->size > 0);
	
	// we'll need to save the data to return it
	Node *start_node = list->head;
	void *data = start_node->data;
	
	// then replace the head with its next node (may be null)
	list->head = list->head->next;

	// if this was the last node in the list, the last also needs to be cleared
	if(list->size == 1) {
		list->last = NULL;
	}

	// the list is now one node smaller
	list->size--;

	// and we're finished with the node holding this data
	free_node(start_node);

	// done!
	return data;
}

// remove and return the final data element in a list
// this operation is O(n), where n is the number of elements in the list
// error if the list is empty (so first ensure list_size() > 0)
void *list_remove_end(List *list) {
	assert(list != NULL);
	assert(list->size > 0);
	
	// we'll need to save the data to return it
	Node *end_node = list->last;
	void *data = end_node->data;
	
	// then replace the last with the second-last node (may be null)
	// (to find this replacement, we'll need to walk the list --- the O(n) bit
	Node *node = list->head;
	Node *prev = NULL;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	list->last = prev;
	
	if(list->size == 1) {
		// if we're removing the last node, the head also needs clearing
		list->head = NULL;
	} else {
		// otherwise, the second-last node needs to drop the removed last node
		prev->next = NULL;
	}

	// the list just got one element shorter
	list->size--;

	// we're finished with the list node holding this data
	free_node(end_node);

	// done!
	return data;
}

// return the number of elements contained in a list
int list_size(List *list) {
	assert(list != NULL);

	return list->size;
}

// returns whether the list contains no elements (true) or some elements (false)
bool list_is_empty(List *list) {
	assert(list != NULL);
	
	return (list->size==0);
}
