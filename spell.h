/* * * * * * *
 * Module for functions that solve the coding tasks from assignment 2
 * 
 * created for COMP20007 Design of Algorithms - Assignment 2, 2018
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#ifndef SPELL_H
#define SPELL_H

#include "list.h"

/*                         DO NOT CHANGE THIS FILE
 * 
 * DO NOT modify function prototypes: you must implement these functions exactly
 * DO NOT add your own function prototypes or other definitions; put additional
 * definitions in spell.c instead
 *
 * We will test your assignment with an unmodified version of this file. Any
 * changes you make will be lost. This may lead to compile errors.
 */

// see Assignment Task 1: Computing edit distance
void print_edit_distance(char *word1, char *word2);

// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word);

// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document);

// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document);

#endif