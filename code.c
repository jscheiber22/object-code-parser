#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Prototypes
int opCodeText(int opCode, int lengthOfLists, int operations[][1]);
int getTA(int bp, int x);
int getAddrMode(int ni);


// Main function
int main(int argc, char *argv[]) {

	int lengthOfLists = 59;
	int indexOfEverything, format;
	//printf("%s", argv[1]); # prints filename of input file

	int operations[][1] = {
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


	char abbrev[][6] = {
		"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP",
		"COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
		"FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
		"JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
		"LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
		"MULR", "NORM", "OR", "RD", "RMO", "RSUB",
		"SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB",
		"STCH", "STF", "STI", "STL","STS", "STSW",
		"STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
		"TD", "TIO", "TIX", "TIXR", "WD"
	};

	int isFormat2[][1] = {
		0,0,1,0,1,0,
		0,1,0,0,1,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,0,0,
		1,0,0,0,1,0,
		1,1,0,0,0,0,
		0,0,0,0,0,0,
		0,0,0,0,1,1,
		0,0,0,1,0
	};

	// immediate/simple	
	char OAT[][15] = {
		"immediate", // (ni == 0x010000)
		"simple", // else bc ni both = 0 or 1
		"indirect" // (ni == 0x020000)
	};

	// pc/base/absolute
	char TAAM[][15] = {
		"pc", // 0x00200
		"base", // 0x00400
		"absolute" // ? direct? // bp == 0x000000 ?
	};

	//
	// if simple -> check for index (x) otherwise don't 
	//



	char header[] = "INSTR FORMAT OAT TAAM OBJ";

	int a = 0x032600;
	int opCode = a & 0xFC0000;
	opCode = 0x90;

	indexOfEverything = opCodeText(opCode, lengthOfLists, operations);

	printf("opcode: %X\n", opCode);	
	//printf("operat: %X\n", *operations[0]);
	//printf("operat: %X\n", 0xB8);

	int TA = getTA(a & 0x00600, a & 0x008000);
	int addrMode = getAddrMode(a & 0x030000);


	// spot for finding format, should divide length of data by 8, then value will == format
	//if (isFormat2[indexOfEverything]) {
	//	format = 2;	
	//} else {
	//	//          n, i     	    x             b,p 
	//	getFormat(a & 0x030000, a & 0x008000, a & 0x006000); 
	//}

	FILE *outFile;
        outFile = fopen("obj_struct.txt", "w+");
	fprintf(outFile, "%s\n", header);
	//fprintf(outFile, "%X\n", opCodeText(opCode, lengthOfLists, operations));
	fprintf(outFile, "%s %d\n", abbrev[indexOfEverything], format);
	fclose(outFile);

	return 0;
}

int getTA(int bp, int x){
	// if e=1 format = 3
//	if (bp & 0x001000){
//		return 3;
//	} else if (
}


int getAddrMode(int ni){
//	if (ni 
}


int opCodeText(int opCode, int lengthOfLists, int operations[][1]) {
	
	for (int i = 0; i < lengthOfLists - 1; i++) {
//		printf("operat: %X\n", *operations[i]);
		if (opCode == *operations[i]) {
//			printf("operat: %X\n", *operations[i]);
    			//return *operations[i];
			return i;
		}
  	}	

	return 0x01; // 1 for fail

}
