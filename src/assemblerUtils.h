/*
 * assemblerUtils.h
 *
 *  Created on: 6 Jun 2013
 *      Author: arjun
 */

#ifndef ASSEMBLERUTILS_H_
#define ASSEMBLERUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef char* string;

string getFile2(int argc, char **argv) {
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

//	uint8_t *instructions;
//	instructions = (a->memory);
//	int i;
//	for (i = 0; i < (size)/4; i++) {
//		int j;
//		for (j = 3; j >= 0; j--) {
//			uint8_t m = *(buffer + (i * 4) + j);
//			instructions[(i * 4) + (3-j)] = m;
//		}
//	}


	/* Close the file */
	fclose(file);
	/* Write entire contents of file to standard out */
	fwrite(buffer, 1, size, stdout);

	return buffer;
}


int getReg(string x) {
	if (strlen(x) < 3) {
		return (x[1] - '0');
	} else {
		return (10 + (x[2] - '0'));
	}
}

uint32_t assgetBits(uint32_t number, int start, int fin) {
	//BIT WILL BE IN THE LSB END
	uint32_t heading = number << (31 - start);
	uint32_t leading = heading >> (31 - (start - fin));
	return leading;
}

uint32_t asssetBit(uint32_t number, int position, int setBit) {
	if (getBits(number, position, position) == setBit) {
		return number;
	} else {
		uint32_t bottom = getBits(number, position - 1, 0);
		uint32_t result = number >> position;
		result ^= 1;
		result <<= position;
		result += bottom;
		return result;
	}
}

//start - finish is read left to right in BIG ENDIAN FORM
//start - finish is INCLUSIVE so for a 4-bit change, diff = (4-bit) - 1
uint32_t setBits(uint32_t original, uint32_t change, int start, int finish) {
  int diff = start - finish;
  uint32_t toBeChanged = original;

  while (diff >= 0) {
    uint32_t newBit = assgetBits(change, diff, diff);

    toBeChanged = asssetBit(toBeChanged, start, newBit);

    start--;
    diff--;
  }

  return toBeChanged;
}

#endif /* ASSEMBLERUTILS_H_ */
