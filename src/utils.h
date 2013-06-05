/*
 * utils.hv
 *
 *  Created on: 1 Jun 2013
 *      Author: arjun
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef char* string;

struct ProcessorMem {
	/*
	 * 0..12 = registers[0..12]
	 * PC = registers[15]
	 * CPSR = registers[16]
	 */
	uint32_t registers[17];
	uint8_t memory[65580];	//GPIO pins will be 65536 .. 65547
							//word 65564-68 will be setting pins output on
							//word 65576-79 will be clearing pins output
};

uint32_t getBits(uint32_t number, int start, int fin) {
	//BIT WILL BE IN THE LSB END
	uint32_t heading = number << (31 - start);
	uint32_t leading = heading >> (31 - (start - fin));
	return leading;
}

uint32_t setBit(uint32_t number, int position, int setBit) {
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

uint32_t getRegisterValue(uint8_t rn, struct ProcessorMem *a) {
	return (a->registers)[rn];
}

bool checkCondition(uint8_t cond, struct ProcessorMem *a) {
	int N = getBits((a->registers)[16], 31, 31);
	int Z = getBits((a->registers)[16], 30, 30);
	int V = getBits((a->registers)[16], 28, 28);

	bool result = false;

	switch (cond) {
	case 0:
		result = (Z == 1);
		break;
	case 1:
		result = (Z == 0);
		break;
	case 10:
		result = (N == V);
		break;
	case 11:
		result = (N != V);
		break;
	case 12:
		result = ((Z == 0) && (N == V));
		break;
	case 13:
		result = ((Z == 1) || (N != V));
		break;
	case 14:
	default:
		result = true;
		break;
	}

	return result;
}

uint32_t getWordMemory(int x, struct ProcessorMem *a) {
	uint32_t result;
	uint32_t m, m1, m2, m3;
	m = (a->memory)[x];
	m1 = (a->memory)[x + 1];
	m2 = (a->memory)[x + 2];
	m3 = (a->memory)[x + 3];
	m = m * (pow(pow(2, 8), 3));
	m1 = m1 * (pow(pow(2, 8), 2));
	m2 = m2 * pow(2, 8);
	result = m + m1 + m2 + m3;
	return result;
}

void storeWordMemory(int x, uint32_t word, struct ProcessorMem *a) {
	a->memory[x] = getBits(word, 31, 24);
	a->memory[x + 1] = getBits(word, 23, 16);
	a->memory[x + 2] = getBits(word, 15, 8);
	a->memory[x + 3] = getBits(word, 7, 0);
}

void resetProcessorMem(struct ProcessorMem *a) {
	memset(a->registers, 0, 17);
	memset(a->memory, 0, 65536);
	storeWordMemory(65536, 538968072, a);
	storeWordMemory(65540, 538968068, a);
	storeWordMemory(65544, 538968064, a);
}

int getDigits(int64_t x) {
	if (x < 10 && x >= 0) {
		return 1;
	} else if (x < 0) {
		return (getDigits(x * (-1))) + 1;
	} else {
		return getDigits(x / 10) + 1;
	}
}

int printProcessorMem(struct ProcessorMem *a) {
	int i;
	printf("%s\n", "Registers:");
	int32_t reg = 0;
	char whitespace[10];
	int j;
	for (i = 0; i < 13; i++) {
		reg = a->registers[i];
		memset(whitespace, 0, 10);
		if (getDigits(reg) < 11) {
			for (j = 0; j < (10 - getDigits(reg)); j++) {
				strcat(whitespace, " ");
			}
			if (i < 10) {
				printf("$%i%s%s%i%s%08x%s\n", i, "  : ", whitespace, reg,
						" (0x", reg, ")");
			} else {
				printf("$%i%s%s%i%s%08x%s\n", i, " : ", whitespace, reg, " (0x",
						reg, ")");
			}
		} else {
			if (i < 10) {
				printf("$%i%s%i%s%08x%s\n", i, "  : ", reg, " (0x", reg, ")");
			} else {
				printf("$%i%s%i%s%08x%s\n", i, " : ", reg, " (0x", reg, ")");
			}
		}
	}

	reg = a->registers[15];
	if (getDigits(reg) < 11) {
		memset(whitespace, 0, 10);
		for (j = 0; j < (10 - getDigits(reg)); j++) {
			strcat(whitespace, " ");
		}
		printf("%s%s%s%i%s%08x%s\n", "PC", "  : ", whitespace, reg, " (0x", reg,
				")");
	} else {

		printf("%s%s%i%s%08x%s\n", "PC", "  : ", reg, " (0x", reg, ")");
	}
	reg = a->registers[16];
	if (getDigits(reg) < 11) {
		memset(whitespace, 0, 10);
		for (j = 0; j < (10 - getDigits(reg)); j++) {
			strcat(whitespace, " ");
		}
		printf("%s%s%s%i%s%08x%s\n", "CPSR", ": ", whitespace, reg, " (0x", reg,
				")");
	} else {
		printf("%s%s%i%s%08x%s\n", "CPSR", ": ", reg, " (0x", reg, ")");
	}

	printf("%s\n", "Non-zero memory:");
	for (i = 0; i < (65536 / 4); i++) {
		if ((getWordMemory((i * 4), a)) != 0) {
			printf("%s%08x%s%s%08x\n", "0x", (i * 4), ": ", "0x",
					getWordMemory((i * 4), a));
		}
	}
	return 0;

}

int checkGPIOpin(uint32_t x) {
	//returns minimum pin
	if (x == 538968072) {
		return 20;
	} else if (x == 538968068) {
		return 10;
	} else if (x == 538968064) {
		return 0;
	} else if (x == 538968092) {
		return 1;
	} else if (x == 538968104) {
		return 2;
	}else{
		return -1;
	}
}


//void clearpin(int x, struct ProcessorMem *a) {
//	//TURNS OUTPUT OF PIN to 0
//	setBit(getWordMemory(65576, a), x, 1);
//	if (getBits(getWordMemory(65564, a), x, x) == 1) {
//		printf("%s", "LED OFF!");
//	}
//}
//
//void setpin(int x, struct ProcessorMem *a) {
//	//TURNS OUTPUT OF PIN to 1
//	clearpin(x, a);
//	printf("%s", "LED OFF!");
//	setBit(getWordMemory(65564, a), x, 1);
//	printf("%s", "LED ON!");
//}

//=====================================================
//STUFF TO REMOVE======================================

void prints(string s) {
	printf("%s\n", s);
}

void printBits(uint32_t x) {
	int i;
	uint32_t mask = 1 << 31;

	for (i = 0; i < 32; ++i) {
		printf("%i", (x & mask) != 0);
		x <<= 1;
	}

	printf("\n");

}

void print(uint32_t x) {
	printf("%u\n", x);
}

#endif /* UTILS_H_ */
