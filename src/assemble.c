#include "assemblerUtils.h"
#include "codeinst.h"

void printH(uint32_t x) {
	printf("%08x\n", x);
}

string* assemblyfile;

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
	printf("%s\n", lines[0]);
//	tok = strtok_r(NULL, " ,", tokens);
//	printf("%s\n", tok);
//	tok = strtok_r(NULL, " ,", tokens);
//	printf("%s\n", tok);





	return EXIT_SUCCESS;
}
