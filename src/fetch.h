/*
 * fetch.h
 *
 *  Created on: 1 Jun 2013
 *      Author: arjun
 */

#ifndef FETCH_H_
#define FETCH_H_

#include "utils.h"

//const int MAX_WORD_SIZE = 32;

uint8_t* getFile(int argc, char **argv, struct ProcessorMem *a) {
	if (argc != 2) {
		fprintf(stderr, "Please specify filename.\n");
		exit(EXIT_FAILURE);
	}

	FILE *file;

	/* Open file in read-only, binary mode */
	if ((file = fopen(argv[1], "rb")) == NULL ) {
		fprintf(stderr, "Failed to open file: %s\n.", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Seek to the end of the file */
	fseek(file, 0, SEEK_END);

	/* Find the current file position (the file size) */
	long size = ftell(file);
	//printf("%d\n", size);
	/* Allocate buffer for the file contents */
	string buffer = malloc(size);

	if (buffer == NULL ) {
		fprintf(stderr, "malloc() failed!\n");
		exit(EXIT_FAILURE);
	}

	/* Seek back to the start of the file, or use rewind() */
	fseek(file, 0, SEEK_SET);

	/* Read entire contents of file into buffer */
	fread(buffer, 1, size, file);


	uint8_t *instructions;
	instructions = (a->memory);
	int i;
	for (i = 0; i < (size)/4; i++) {
		int j;
		for (j = 3; j >= 0; j--) {
			uint8_t m = *(buffer + (i * 4) + j);
			instructions[(i * 4) + (3-j)] = m;
		}
	}


	/* Close the file */
	fclose(file);
	/* Write entire contents of file to standard out */
	//fwrite(buffer, 1, size, stdout);

	free(buffer);

	return EXIT_SUCCESS;
}

#endif /* FETCH_H_ */
