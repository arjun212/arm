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

uint32_t assembleDataProcess(struct data_process* a) {
uint32_t result = 0;
result = setBits(result, a->cond, 31, 28);
result = setBits(result, a->immed, 25, 25);
result = setBits(result, a->opCode, 24, 21);
result = setBits(result, a->set, 20, 20);
result = setBits(result, a->rn, 19, 16);
result = setBits(result, a->rd, 15, 12);
result = setBits(result, a->op2, 11, 0);
return result;
}
uint32_t assembleMultiply(struct multiply* a) {
uint32_t result = 0;
result = setBits(result, a->cond, 31, 28);
result = setBits(result, a->acc, 21, 21);
result = setBits(result, a->set, 20, 20);
result = setBits(result, a->rd, 19, 16);
result = setBits(result, a->rn, 15, 12);
result = setBits(result, a->rs, 11, 8);
result = setBits(result, 9, 7, 4);
result = setBits(result, a->rm, 3, 0);
return result;
}
uint32_t assembleSingleData(struct single_data* a) {
uint32_t result = 0;
result = setBits(result, a->cond, 31, 28);
result = setBits(result, 1, 26, 26);
result = setBits(result, a->immed, 25, 25);
result = setBits(result, a->pre, 24, 24);
result = setBits(result, a->up, 23, 23);
result = setBits(result, a->load, 20, 20);
result = setBits(result, a->rn, 19, 16);
result = setBits(result, a->rd, 15, 12);
result = setBits(result, a->offset, 11, 0);
return result;
}
uint32_t assembleBranch(struct branch* a) {
uint32_t result = 0;
result = setBits(result, a->cond, 31, 28);
result = setBits(result, 5, 27, 25);
result = setBits(result, a->offset, 24, 0);
return result;
}

#endif /* ASSEMBLECODE_H_ */
