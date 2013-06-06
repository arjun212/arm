#include "assemblerUtils.h"
#include "codeinst.h"

void printH(uint32_t x) {
	printf("%08x\n", x);
}

string* assemblyfile;

bool isLabel(string x) {
	int length = strlen(x);
	if ((x[0] >= 'a' && x[0] <= 'z') || (x[0] >= 'A' && x[0] <= 'Z')) {
		return (x[length - 1] == ':');
	} else {
		return 0;
	}
}

int main(int argc, char **argv) {
	string a = getFile2(argc, argv);
	int i = 0;
	int counter = 0;
	while (i < strlen(a)) {
		if (a[i] == '\n') {
			counter++;
		}
		i++;
	}
	string fline = strtok(a, "\n");
	string lines[counter+1];
	i = 0;
	while (i < counter) {
//		printf("%s\n", fline);
		lines[i] = malloc(strlen(fline));
		lines[i] = fline;
		fline = strtok(NULL, "\n");
		i++;
	}
	lines[2] = "foo:";
	i = 1;

	listptr symboltable = newList(newElem("", 0));
	if (isLabel(lines[0])) {
		addElem(newElem(lines[0], 0), symboltable);
	}

	while (i < counter+1) {
		if (isLabel(lines[i])) {
			addElem(newElem(lines[i], i), symboltable);
		}
		i++;
	}

	printf("%d\n", numofelems(symboltable) - 1);

	return EXIT_SUCCESS;
}
