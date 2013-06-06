#include "assemblerUtils.h"
#include "codeinst.h"

void printH(uint32_t x) {
	printf("%08x\n", x);
}

string* assemblyfile;

string* tokenisefile(string buf) {
	//MAX LINE LENGTH = 511, from SPEC
	string line = malloc(511);
	int i;
	while (i < 5) {

	}
//	while (i < 2) {
//		linesize = 0;
//		ch = 0;
//		while (ch != '\n') {
//			ch = buf[linesize];
//			linesize++;
//		}
//		result[linecounter] = malloc(linesize);
//		ch = 0;
//		linesize = 0;
//		while (ch != '\n') {
//			ch = buf[linesize];
//			(result[linecounter])[linesize] = ch;
//			linesize++;
//		}
//		linecounter++;
//	}
//	i++;
//	return result;
}

bool isLabel(string x) {
	int length = strlen(x);
	if ((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) {
		return (x[length-1] == ':');
	} else {
		return 0;
	}
}

int main(int argc, char **argv) {
	string lines[2];
	lines[0] = "mov r1, #1";
	lines[1] = "add r2, r1, #2";
	string a = getFile2(argc, argv);

	//printf("%s\n", convertToInstruction(lines[0]));


























	return EXIT_SUCCESS;
}
