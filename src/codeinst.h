/*
 * codeinst.h
 *
 *  Created on: 6 Jun 2013
 *      Author: arjun
 */

#ifndef CODEINST_H_
#define CODEINST_H_

#include "map.h"
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
	//OUTPUTS OPCODE
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
	//COND /
	//OPCODE /

}

uint32_t branchLookupTable(string x) {
	//OUTPUTS just a cond, to distinguish between instructions
	listptr p = newList(newElem("beq", 0));
	addElem(newElem("bne", 1), p);
	addElem(newElem("bge", 10), p);
	addElem(newElem("blt", 11), p);
	addElem(newElem("bgt", 12), p);
	addElem(newElem("ble", 13), p);
	addElem(newElem("b", 14), p);
	return getElem(x, p);
}

uint32_t multiplyLookupTable(string x) {
	//OUTPUTS 'A' BIT (Bit 21)
	listptr p = newList(newElem("mul", 0));
	addElem(newElem("mla", 1), p);
	return getElem(x, p);
}

uint32_t singleLookupTable(string x) {
	//OUTPUTS 'L' BIT (Bit 20)
	listptr p = newList(newElem("ldr", 1));
	addElem(newElem("str", 0), p);
	return getElem(x, p);
}

uint32_t setDataLookupTable(string x) {
	//OUTPUTS 'A' BIT (Bit 21)
	listptr p = newList(newElem("tst", 1));
	addElem(newElem("teq", 1), p);
	addElem(newElem("cmp", 1), p);
	return getElem(x, p);
}

ins_type getInstructionType(string x) {
	if (dataLookupTable(x) != 32) {
		return data_process;
	} else if (branchLookupTable(x) != 32) {
		return branch;
	} else if (singleLookupTable(x) != 32) {
		return single_data;
	} else if (multiplyLookupTable(x) != 32) {
		return multiply;
	} else if (strncmp(x, "andeq", 5) == 0) {
		return special1;
	} else {
		return special2;
	}
}

//string spec1 = "andeq r0 r0 r0";


//WILL RETURN uint32_t
string convertToInstruction(string x) {
	//string a = FIRST TOKEN OF STRING x
	//TOKEN[1] = FIRST WORD OF INSTRUCTION
	string* token;
	ins_type instructionType = getInstructionType(a);
	if (getInstructionType(token[1]) == 0) {
		struct data_process s;
		s.cond = 14;
		if (token[1])
		s.immed = ;
		s.op2 = ;
		s.opCode = ;
		s.rd = ;
		s.rn = ;
		s.set
		//GET TOKENS FOR DATA PROCESS
		//send to assemble DataProcess
//	DATA PROCESS
	} else if (getInstructionType(token[1]) == 1) {
		struct data_process s;
//	MULTIPLY
	} else if (getInstructionType(token[1]) == 2) {
		struct data_process s;
//	SINGLE DATA
	} else if (getInstructionType(token[1]) == 3) {
		struct data_process s;
//	BRANCH
	} else if (getInstructionType(token[1]) == 4) {
		struct data_process s;
//	SPECIAL1
	} else {
		struct data_process s;
//	SPECIAL2
	}
}

#endif /* CODEINST_H_ */
