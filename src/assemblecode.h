/*
 * assemblecode.h
 *
 *  Created on: 6 Jun 2013
 *      Author: arjun
 */

#ifndef ASSEMBLECODE_H_
#define ASSEMBLECODE_H_

#include "assemblerUtils.h"

typedef enum ins_type {
	data_process, multiply, single_data, branch, special1, special2
} ins_type;

struct data_process {
	uint8_t cond;
	bool immed;
	uint8_t opCode;
	bool set;
	uint8_t rn;
	uint8_t rd;
	uint16_t op2;

};

struct multiply {
	uint8_t cond;
	bool acc;
	bool set;
	uint8_t rd;
	uint8_t rn;
	uint8_t rs;
	uint8_t rm;
};

struct single_data {
	uint8_t cond;
	bool immed;
	bool pre;
	bool up;
	bool load;
	uint8_t rn;
	uint8_t rd;
	uint16_t offset;
};

struct branch {
	uint8_t cond;
	uint32_t offset;
} branch;

uint32_t assemblrbranch(branch* a) {
	uint32_t result = 0;
	result = setBits(result)
	return result;
}

#endif /* ASSEMBLECODE_H_ */
