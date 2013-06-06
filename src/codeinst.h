/*
 * codeinst.h
 *
 *  Created on: 6 Jun 2013
 *      Author: arjun
 */

#ifndef CODEINST_H_
#define CODEINST_H_

#include "map.h"

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

typedef enum m_inst {
	mul, mla
} m_inst;

typedef enum sd_inst {
	ldr, str
} sd_inst;

typedef enum ins_type {
	data_process, multiply, single_data, branch, special1, special2
} ins_type;

uint32_t dataLookupTable(string x) {
	listptr p = newList(newElem("and", 0));
	addElem(newElem("eor", 1), p);
	addElem(newElem("sub", 2), p);
	addElem(newElem("rsb", 3), p);
	addElem(newElem("add", 4), p);
	addElem(newElem("orr", 12), p);
	addElem(newElem("mov", 13), p);
	addElem(newElem("tst", 8), p);
	addElem(newElem("teq", 9), p);
	addElem(newElem("cmp", 10), p);
	return getElem(x, p);
}

uint32_t branchLookupTable(string x) {
	listptr p = newList(newElem("beq", 0));
	addElem(newElem("bne", 1), p);
	addElem(newElem("bge", 10), p);
	addElem(newElem("blt", 11), p);
	addElem(newElem("bgt", 12), p);
	addElem(newElem("ble", 13), p);
	addElem(newElem("b", 14), p);
	return getElem(x, p);
}



//ins_type getInstructionType(string x) {
//if (dataLookupTable(x) == 32) {
//	return data_process;
//	} else if (branchLookupTable(x) == 32) {
//return branch;
//	} else if () {
//		//SINGLE DATA TRANSFER
//	} else if() {
//		 //MULTIPLY
//	} else if() {
//		//SPECIAL CASE 1
//	} else {
//		//SPECIAL CASE 2
//	}
//}

//string spec1 = "andeq r0 r0 r0";
//
//uint32_t codeDataProcess(string* tokens) {
//
//}
//uint32_t codeMultiply(string* tokens) {
//
//}uint32_t codeSingleData(string* tokens) {
//
//}uint32_t codeBranch(string* tokens) {
//
//}uint32_t SPECIAL2(string* tokens) {
//
//}

//WILL RETURN uint32_t
//string convertToInstruction(string x) {
	//string* tokens = TOKENISE(x);
	//string tokens = strtok(x, " ,");
	//return tokens;
//	string token[5];
//	if (getInstructionType(token[1]) == 0) {
//	DATA PROCESS
//	} else if (getInstructionType(token[1]) == 1) {
//	MULTIPLY
//	} else if (getInstructionType(token[1]) == 2) {
//	SINGLE DATA
//	} else if (getInstructionType(token[1]) == 3) {
//	BRANCH
//	} else if (getInstructionType(token[1]) == 4) {
//	SPECIAL1
//	} else {
//	SPECIAL2
//	}
//}

#endif /* CODEINST_H_ */
