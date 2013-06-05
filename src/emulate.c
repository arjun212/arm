#include "fetch.h"
#include "decode.h"
#include "execute.h"

int mainn(int argc, char **argv) {
//	char str[] = ' ';
//	str = snprintf(str, 16, "%d", -2147483648);
//	prints(str);
	struct ProcessorMem Processor;
	resetProcessorMem(&Processor);
	getFile(argc, argv, &Processor);
	//printProcessorMem(&Processor);
	bool haltfound = 0;
	uint32_t instruction;
	struct branch bdata;
	struct multiply mdata;
	struct single_data sdata;
	struct data_process ddata;
	ins_type decodedtype;
	instruction = getWordMemory((Processor.registers[15]), &Processor);	//INITIAL FETCH
	Processor.registers[15] += 4;
	while (!haltfound) {
		decodedtype = getType(instruction);
//		printBits(instruction);
//
//		switch (decodedtype) {
//		case (0):
//			prints("data decoded");
//			break;
//		case (1):
//			prints("multiply decoded");
//			break;
//		case (2):
//			prints("single decoded");
//			break;
//		case (3):
//			prints("branch decoded");
//			break;
//		case (4):
//			prints("halting");
//			break;
//		}
//
//		switch (decodedtype) {
//		case (0):
//			prints("data executed");
//			break;
//		case (1):
//			prints("multiply executed");
//			break;
//		case (2):
//			prints("single executed");
//			break;
//		case (3):
//			prints("branch executed");
//			break;
//		case (4):
//			prints("halting executed");
//			break;
//		}

		switch (decodedtype) {											//DECODE
		case (0):		//DATA_PROCESS
		{
			initData(&ddata, instruction);
			break;
		}
		case (1):		//MUTIPLY
		{
			initMutiply(&mdata, instruction);
			break;
		}
		case (2):		//SINGLE DATA
		{
			initSingle(&sdata, instruction);
			break;
		}
		case (3):		//BRANCH
		{
			initBranch(&bdata, instruction);
			break;
		}
		case (4): 		//HALT
		{
			haltfound = 1;
			break;
		}
		}

		instruction = getWordMemory((Processor.registers[15]), &Processor);	//FETCH
		Processor.registers[15] += 4;
		//print(Processor.registers[2]);
		switch (decodedtype) {										//EXECUTE
		case (0):		//DATA_PROCESS
		{
			executeData(&ddata, &Processor);
			break;
		}
		case (1):		//MUTIPLY
		{
			executeMultiply(&mdata, &Processor);
			break;
		}
		case (2):		//SINGLE DATA
		{
			executeSingle(&sdata, &Processor);
			break;
		}
		case (3):		//BRANCH
		{
			{
				executeBranch(&bdata, &Processor);
				instruction = getWordMemory((Processor.registers[15]),
						&Processor);	//FETCH
				break;
			}
			case (4):
			//HALT
			{
				break;
			}
		}
		}
//		printProcessorMem(&Processor);
	}

	printProcessorMem(&Processor);
	return 0;

}
