#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes
int opCodeText(int opCode, int lengthOfLists, int operations[][2]);


// Main function
int main(int argc, char *argv[]) {

	int lengthOfLists = 59;

	int operations[][2] = {
                0x18, 0x58, 0x90, 0x40, 0xB4, 0x28,
                0x88, 0xA0, 0x24, 0x64, 0x9C, 0xC4,
                0xC0, 0xF4, 0x3C, 0x30, 0x34, 0x38,
                0x48, 0x00, 0x68, 0x50, 0x70, 0x08,
                0x6C, 0x74, 0x04, 0xD0, 0x20, 0x60,
                0x98, 0xC8, 0x44, 0xD8, 0xAC, 0x4C,
                0xA4, 0xA8, 0xF0, 0xEC, 0x0C, 0x78,
                0x54, 0x80, 0xD4, 0x14, 0x7C, 0xE8,
                0x84, 0x10, 0x1C, 0x5C, 0x94, 0xB0,
                0xE0, 0xF8, 0x2C, 0xB8, 0xDC
        };


	//printf("%s", argv[1]); # prints filename of input file

	char header[] = "INSTR FORMAT OAT TAAM OBJ";

	int a = 0x032600;
	int opCode = a & 0xFC0000;
	opCode = 0xDC;

	printf("opcode: %X\n", opCode);	
	//printf("operat: %X\n", *operations[0]);
	//printf("operat: %X\n", 0xB8);

	FILE *outFile;
        outFile = fopen("obj_struct.txt", "w+");
	fprintf(outFile, "%s\n", header);
	fprintf(outFile, "%X\n", opCodeText(opCode, lengthOfLists, operations));
	fclose(outFile);

	return 0;
}


int opCodeText(int opCode, int lengthOfLists, int operations[][2]) {
	for (int i = 0; i < lengthOfLists - 1; i++) {
		printf("operat: %X\n", *operations[i]);
		if (opCode == *operations[i]) {
    			return *operations[i];
		}
  	}	

	return 0x01; // 1 for fail

}
