/*
 * execute.h
 *
 *  Created on: 1 Jun 2013
 *      Author: arjun
 */

#ifndef EXECUTE_H_
#define EXECUTE_H_

#include "utils.h"
#include "barrelshifter.h"

uint32_t immediateValue(uint16_t operand, bool* carry, struct ProcessorMem *a) {
	bsdata final;
	uint32_t result = 0;
	uint8_t rfactor = (operand) >> 8; 	//rotationfactor
	rfactor = (rfactor * 2);
	result = getBits(operand, 7, 0);					//EXTENDS to 32 BITS
	final = ror(result, rfactor, a);
	*carry = final.carry;		//MAY NEED TO ADD A CARRYOUT BIT HERE
	return final.result;
}

uint32_t registerValue(uint16_t operand, bool* carry, struct ProcessorMem *a) {
	bsdata final;
	uint8_t rm = getBits(operand, 3, 0);	//TAKE 4 LSB or OP2
	uint32_t result = getRegisterValue(rm, a);
	//GET SHIFT TYPE ================================
	uint8_t shifttype = getBits(operand, 6, 5);
	//===============================================
	uint8_t shiftamount = 0;
	if (getBits(operand, 4, 4)) {	//SHIFT SPECIFIEC BY REGISTER
		uint8_t rs = getBits(operand, 11, 8);
		shiftamount = getRegisterValue(rs, a);
	} else {	//SHIFT SPECIFIED BY CONSTANT
		shiftamount = getBits(operand, 11, 7);
	}

	switch (shifttype) {
	case 0:		//00 - logical left
		final = lsl(result, shiftamount, a);
		break;
	case 1:		//01 - logical right
		final = lsr(result, shiftamount, a);
		break;
	case 2:		//10 - arithmetic right
		final = asr(result, shiftamount, a);
		break;
	case 3:		//11 - rotate right
		final = ror(result, shiftamount, a);
		break;
	}
	*carry = final.carry;
	return final.result;
}

int executeData(struct data_process *data, struct ProcessorMem *a) {
	//if condition true then execute instruction
	if (checkCondition(data->cond, a)) {
		uint32_t operand1 = getRegisterValue(data->rn, a);
		uint32_t operand2 = 0;
		bool carryout = getBits((a->registers)[16], 29, 29);
		if (data->immed) {	//OPERAND2 = IMMEDIATE VALUE
			operand2 = immediateValue(data->op2, &carryout, a);
		} else {	//OPERAND2 = REGISTER VALUE
			operand2 = registerValue(data->op2, &carryout, a);
		}
		uint8_t destinationregister = (data->rd);
		uint32_t result = (a->registers)[destinationregister];
		switch (data->opCode) {
		case 0:			//0000 - and
			result = operand1 & operand2;
			break;
		case 1:			//0001 - eor
			result = operand1 ^ operand2;
			break;
		case 2:			//0010 - sub
			result = operand1 - operand2;
			break;
		case 3:			//0011 - rsb
			result = operand2 - operand1;
			break;
		case 4:			//0100 - add
			result = operand1 + operand2;
			break;
		case 8:			//1000 - tst
			result = operand1 & operand2;	//RESULT NOT WRITTEN
			break;
		case 9:			//1001 - teq
			result = operand1 ^ operand2;	//RESULT NOT WRITTEN
			break;
		case 10:		//1010 - cmp
			result = operand1 - operand2;	//RESULT NOT WRITTEN
			break;
		case 12:		//1100 - orr
			result = operand1 | operand2;
			break;
		case 13:		//1101 - mov
			result = operand2;
			break;

		}

		if (!((data->opCode == 8) || (data->opCode == 9) || (data->opCode == 10))) {
			(a->registers)[destinationregister] = result;
		}

		if (data->set) {
			(a->registers)[16] = setBit((a->registers)[16], 29, carryout);

			if (!result) {
				(a->registers)[16] = setBit((a->registers)[16], 30, 1);
			}

			(a->registers)[16] = setBit((a->registers)[16], 31,
					getBits(result, 31, 31));
			//	0100
		}
	}
	return 0;
}

int executeMultiply(struct multiply *data, struct ProcessorMem *a) {
	//if condition true then execute instruction
	if (checkCondition(data->cond, a)) {
		uint32_t result;
		if (data->acc) {
			result = (a->registers)[(data->rm)] * (a->registers)[(data->rs)]
					+ (a->registers)[(data->rn)];
		} else {
			result = (a->registers)[(data->rm)] * (a->registers)[(data->rs)];
		}

		(a->registers)[(data->rd)] = result;

		if (data->set) {
			if (result == 0) {
				(a->registers)[15] = setBit((a->registers)[15], 30, 1);
			}
			(a->registers)[15] = setBit((a->registers)[15], 31,
					getBits(result, 31, 31));
		}
	}
	return 0;
}

int executeSingle(struct single_data *data, struct ProcessorMem *a) {
//if condition true then execute instruction
	if (checkCondition(data->cond, a)) {
		uint32_t offset;
		bool carryout;
		if (data->immed) {
			offset = registerValue(data->offset, &carryout, a);
			//		offset = data->offset;
		} else {
			offset = immediateValue(data->offset, &carryout, a);
		}

		int32_t brwo;	//Base reg with offset
		if (data->up) {
			brwo = a->registers[data->rn] + offset;
		} else {
			brwo = a->registers[data->rn] - offset;
		}

		bool gpiopin = 0;
		uint32_t gpioaddress = 0;
		if (checkGPIOpin(brwo) > 0) {
			gpioaddress = brwo;
			brwo -= 538902528;
			gpiopin = 1;
		}


		if ((!((brwo > 65576)) || (brwo <= 0))) {
			if (data->pre) {
				if (data->load) {
					//LOADED FROM MEMORY
					(a->registers)[data->rd] = getWordMemory(brwo, a);
					if (gpiopin) {
						printf("%s%i%s%i%s\n", "One GPIO pin from ",
								checkGPIOpin(gpioaddress), " to ",
								checkGPIOpin(gpioaddress) + 9,
								" has been accessed");
					}
				} else {
					//SAVED TO MEMORY
					storeWordMemory(brwo, a->registers[data->rd], a);
					if ((gpiopin) && (checkGPIOpin(gpioaddress) == 1)) {
						printf("%s\n", "LED ON!");
						storeWordMemory(brwo, 1, a);
					} else if ((gpiopin) && (checkGPIOpin(gpioaddress) == 2)) {
						if (getWordMemory(brwo-12, a) != 0) {
							printf("%s\n", "LED OFF!");
						}
						storeWordMemory(brwo-12, 0, a);
					} else if (gpiopin) {
						printf("%s%i%s%i%s\n", "One GPIO pin from ",
								checkGPIOpin(gpioaddress), " to ",
								checkGPIOpin(gpioaddress) + 9,
								" has been accessed");
					}
				}

			} else {

				if (data->load) {
					//LOADED FROM MEMORY
					(a->registers)[data->rd] = getWordMemory(
							a->registers[data->rn], a);
					if (gpiopin) {
						printf("%s%i%s%i%s\n", "One GPIO pin from ",
								checkGPIOpin(gpioaddress), " to ",
								checkGPIOpin(gpioaddress) + 9,
								" has been accessed");
					}
				} else {
					//SAVED TO MEMORY
					storeWordMemory((a->registers)[data->rn],
							a->registers[data->rd], a);
				}
				a->registers[data->rn] = brwo;
				if ((gpiopin) && (checkGPIOpin(gpioaddress) == 1)) {
					printf("%s\n", "LED ON!");
				} else if ((gpiopin) && (checkGPIOpin(gpioaddress) == 2)) {
					printf("%s\n", "LED OFF!");
				} else if (gpiopin) {
					printf("%s%i%s%i%s\n", "One GPIO pin from ",
							checkGPIOpin(gpioaddress), " to ",
							checkGPIOpin(gpioaddress) + 9,
							" has been accessed");
				}

			}
		} else {
			if (data->up) {
				uint32_t address = a->registers[data->rn] + offset;
				if (checkGPIOpin(address) < 0) {
					printf("%s%08x\n",
							"Error: Out of bounds memory access at address 0x",
							address);
				}
			} else {
				uint32_t address = a->registers[data->rn] - offset;
				if (checkGPIOpin(address) < 0) {
					printf("%s%08x\n",
							"Error: Out of bounds memory access at address 0x",
							address);
				}
			}
		}

	}

	return 0;
}

int executeBranch(struct branch *data, struct ProcessorMem *a) {
	int32_t extension = data->offset << 8;
	int32_t currentPC = (a->registers[15]);
	extension = asr(extension, 6, a).result;
//if condition true then execute instruction
	if (checkCondition(data->cond, a)) {
		currentPC += extension;
		(a->registers[15]) = currentPC;
	} else {
		(a->registers)[15] -= 4;
	}
	return 0;
}

#endif /* EXECUTE_H_ */
