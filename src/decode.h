/*
 * decode.h
 *
 *  Created on: 1 Jun 2013
 *      Author: arjun
 */

#ifndef DECODE_H_
#define DECODE_H_

#include "utils.h"

typedef enum ins_type {
	data_process, multiply, single_data, branch, halt
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
};

ins_type getType(uint32_t instr) {
	if (getBits(instr, 27, 26) == 2) {
		return branch;
	} else if (getBits(instr, 27, 26) == 1) {
		return single_data;
	} else if ((getBits(instr, 7, 4) == 9)  && (getBits(instr, 27, 22) == 0)) {
		return multiply;
	} else if (instr == 0) {
		return halt;
	} else {
		return data_process;
	}
}

void initBranch(struct branch *a, uint32_t inst) {
	a->cond = getBits(inst, 31, 28);
	a->offset = getBits(inst, 23, 0);
}

void initSingle(struct single_data *a, uint32_t inst) {
	a->cond = getBits(inst, 31, 28);
	a->immed = getBits(inst, 25, 25);
	a->load = getBits(inst, 20, 20);
	a->offset = getBits(inst, 11, 0);
	a->pre = getBits(inst, 24, 24);
	a->rd = getBits(inst, 15, 12);
	a->rn = getBits(inst, 19, 16);
	a->up = getBits(inst, 23, 23);
}

void initMutiply(struct multiply *a, uint32_t inst) {
	a->cond = getBits(inst, 31, 28);
	a->acc = getBits(inst, 21, 21);
	a->rd = getBits(inst, 19, 16);
	a->rm = getBits(inst, 3, 0);
	a->rn = getBits(inst, 15, 12);
	a->rs = getBits(inst, 11, 8);
	a->set = getBits(inst, 20, 20);

}

void initData(struct data_process *a, uint32_t inst) {
	a->cond = getBits(inst, 31, 28);
	a->immed = getBits(inst, 25, 25);
	a->op2 = getBits(inst, 11, 0);
	a->opCode = getBits(inst, 24, 21);
	a->rd = getBits(inst, 15, 12);
	a->rn = getBits(inst, 19, 16);
	a->set = getBits(inst, 20, 20);

}

#endif /* DECODE_H_ */
