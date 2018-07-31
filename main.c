/* * * * * * * * *
 * Main program:
 * reads command line options, processes input data, calls assignment functions
 * 
 * created for COMP20007 Design of Algorithms - Assignment 2, 2018
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "spell.h"

/*                         DO NOT CHANGE THIS FILE
 * 
 * DO NOT modify any of the constants, types, functions or anything else
 * defined inside this file.
 *
 * We will test your assignment with an unmodified version of this file. Any
 * changes you make will be lost. This may lead to compile errors.
 */

#define MAXIMUM_WORD_LEN 256

// enumeration of the tasks in order of their numbers
typedef enum task {
	TASK_NONE  = 0,
	TASK_DIST  = 1,
	TASK_EDITS = 2,
	TASK_CHECK = 3,
	TASK_SPELL = 4,
} Task;

// struct to store the command line options
typedef struct options {
	int  invalid;
	Task task;
	char *word1;
	char *word2;
	FILE *dicfile;
	FILE *docfile;
} Options;

// helper functions
Options get_options(int argc, char **argv);
List *read_word_list(FILE *file);
void free_word_list(List *list);

// program entry point
int main(int argc, char **argv) {
	
	// parse and validate command line options
	Options options = get_options(argc, argv);
	if (options.invalid) {
		exit(EXIT_FAILURE);
	}

	// branch to relevant function depending on execution mode
	if (options.task == TASK_DIST) {
		print_edit_distance(options.word1, options.word2);

	} else if (options.task == TASK_EDITS) {
		print_all_edits(options.word1);

	} else if (options.task == TASK_CHECK || options.task == TASK_SPELL) {
		// prepare dictionary and document
		List *dictionary = read_word_list(options.dicfile);
		List *document   = read_word_list(options.docfile);

		if (options.task == TASK_CHECK) {
			print_checked(dictionary, document);

		} else { // options.task == TASK_SPELL
			print_corrected(dictionary, document);
		}

		// clean up
		free_word_list(dictionary);
		free_word_list(document);
	}

	// done!
	exit(EXIT_SUCCESS);
}


//                * * * PARSING COMMAND LINE ARGUMENTS * * *


Task strtotask(char *str);

// read command line options into Options struct
Options get_options(int argc, char **argv) {
	
	// create the Options structure with defaults
	Options options = {
		.task    = TASK_NONE,
		.word1   = NULL,
		.word2   = NULL,
		.dicfile = NULL,
		.docfile = NULL,
		.invalid = 0 // false
	};

	// look for the task argument to determine remaining behaviour
	if (argc >= 2) {
		options.task = strtotask(argv[1]);
	}
	
	// then branch based on task to process remaining arguments
	int argc_remaining = argc - 2; // don't count the program name or task

	if (options.task == TASK_NONE) {
		// no task or incorrect task specified!
		fprintf(stderr,
			"please specify which task to run with the first argument:\n");
		fprintf(stderr, " dist:  computing edit distance        (task 1)\n");
		fprintf(stderr, " edits: enumerating all possible edits (task 2)\n");
		fprintf(stderr, " check: spell checking                 (task 3)\n");
		fprintf(stderr, " spell: spelling correction            (task 4)\n");
		options.invalid = 1; // true
	

	} else if (options.task == TASK_DIST) {
		if (argc_remaining == 2) {
			options.word1 = argv[2];
			options.word2 = argv[3];
		} else {
			// not the right number of arguments!
			fprintf(stderr,
				"argument error: please provide exactly two string arguments "
				"for computing edit distance (task 1).\n");
			options.invalid = 1; // true
		}

	} else if (options.task == TASK_EDITS) {
		if (argc_remaining == 1) {
			options.word1 = argv[2];
		} else {
			// not the right number of arguments!
			fprintf(stderr,
				"argument error: please provide exactly one string argument "
				"for enumerating all possible edits (task 2).\n");
			options.invalid = 1; // true
		}

	} else if (options.task == TASK_CHECK || options.task == TASK_SPELL) {
		if (argc_remaining == 1) {
			options.dicfile = fopen(argv[2], "r");
			if (!options.dicfile) {
				perror("error opening dictionary file");
				options.invalid = 1; // true
			}

			options.docfile = stdin;

		} else if (argc_remaining == 2) {
			options.dicfile = fopen(argv[2], "r");
			if (!options.dicfile) {
				perror("error opening dictionary file");
				options.invalid = 1; // true
			}

			options.docfile   = fopen(argv[3], "r");
			if (!options.docfile) {
				perror("error opening document file");
				options.invalid = 1; // true
			}

		} else {
			// not the right number of arguments!
			fprintf(stderr,
				"argument error: please provide one or two filename arguments "
				"for spell checking or spelling corrections (tasks 3 / 4).\n");
			options.invalid = 1; // true
		}
	}
	
	return options;
}

Task strtotask(char *str) {
	if (strcmp("dist",  str) == 0 || strcmp("1", str) == 0) {
		return TASK_DIST;
	}
	if (strcmp("edits", str) == 0 || strcmp("2", str) == 0) {
		return TASK_EDITS;
	}
	if (strcmp("check", str) == 0 || strcmp("3", str) == 0) {
		return TASK_CHECK;
	}
	if (strcmp("spell", str) == 0 || strcmp("4", str) == 0) {
		return TASK_SPELL;
	}
	return TASK_NONE;
}



//              * * * PROCESSING TEXT INPUT FILES * * *


int is_valid_word(char *word, int len);

List *read_word_list(FILE *file) {
	List *list = new_list();

	int  lines_read = 0;
	char line[MAXIMUM_WORD_LEN];

	// read into line buffer until EOF (or read error):
	while (fgets(line, MAXIMUM_WORD_LEN, file)) {
		int linelen = strlen(line);
		lines_read++;
		
		if (linelen == 1) {
			// a completely blank line: consider this the end of input
			break;
		}
		
		// otherwise, there is at least one character. great!
		// strip the newline if it exists.
		if (line[linelen-1] == '\n') {
			line[linelen-1] = '\0';
			linelen--;
		}

		// we should confirm that the word contains only lower case alphabetic
		// characters
		if (is_valid_word(line, linelen)) {
			// and if it does, save the word and add it to the list!
			char *word = malloc((1 + linelen) * sizeof (char));
			assert(word);
			strcpy(word, line);
			list_add_end(list, word);

		} else {
			// but if it doesn't, warn the user and skip this word
			fprintf(stderr,
				"warning: line %d of input has invalid word \"%s\". skipped.\n",
				lines_read, line);
			continue;
		}
	}

	return list;
}

int is_valid_word(char *word, int len) {
	for (int i = 0; i < len; i++) {
		if (word[i] < 'a' || word[i] > 'z') {
			// an invalid letter!
			return 0; // false
		}
	}
	// no invalid letters!
	return 1; // true
}

void free_word_list(List *list) {
	// free the words in the list (they were malloced by this module)
	Node *node = list->head;
	while (node) {
		free(node->data);
		node->data = NULL;
		node = node->next;
	}

	free_list(list);
}
