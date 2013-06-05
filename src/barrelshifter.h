/*
 * barrelshifter.h
 *
 *  Created on: 2 Jun 2013
 *      Author: arjun
 */

#ifndef BARRELSHIFTER_H_
#define BARRELSHIFTER_H_

#include "utils.h"

typedef struct bsdata {
	bool carry;
	uint32_t result;
} bsdata;

bsdata lsl(uint32_t val, uint8_t shifts, struct ProcessorMem *a) {
	bool carry = getBits(a->registers[16],29,29);
	bsdata answer;
	if (shifts != 0) {
		carry = getBits(val, 32 - shifts, 32 - shifts);
	}

	val <<= shifts;
	answer.carry = carry;
	answer.result = val;
	return answer;
}

bsdata lsr(uint32_t val, uint8_t shifts, struct ProcessorMem *a) {
	bool carry = getBits(a->registers[16],29,29);
	bsdata answer;
	if (shifts != 0) {
		carry = getBits(val, shifts-1, shifts-1);
	}
	val >>= shifts;

	answer.carry = carry;
	answer.result = val;
	return answer;
}

bsdata asr(uint32_t val, uint8_t shifts, struct ProcessorMem *a) {
	bool carry  = getBits(a->registers[16],29,29);
	bsdata answer;
	if (shifts != 0) {
		carry = getBits(val, shifts - 1, shifts - 1);
	}
	bool signbit = getBits(val, 31, 31);
	uint32_t result = val >> shifts;
	int i;
	for (i = 0; i <= shifts; i++) {
		result = setBit(result, 31 - i, signbit);
	}
	val = result;

	answer.carry = carry;
	answer.result = val;
	return answer;
}

bsdata ror(uint32_t val, uint8_t shifts, struct ProcessorMem *a) {
	bool carry  = getBits(a->registers[16],29,29);
	bsdata answer;
	if (shifts != 0) {
		carry = getBits(val, shifts - 1, shifts - 1);
	}
	uint32_t carryon = val;
	carryon = carryon << (32 - shifts);
	uint32_t result = val >> shifts;
	int i;
	for (i = 0; i < shifts; i++) {
		result = setBit(result, 31 - i, getBits(carryon, 31 - i, 31 - i));
	}
	val = result;

	answer.carry = carry;
	answer.result = val;
	return answer;
}

#endif /* BARRELSHIFTER_H_ */
