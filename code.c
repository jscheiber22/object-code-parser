#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Prototypes
int opCodeText(int opCode, int lengthOfLists, int operations[][1]);
int getTAAM(int bp);
int getOAT(int ni);
int doTheThing(long a);


// Main function
int main(int argc, char *argv[]) {

	// //printf("%s", argv[1]); # prints filename of input file

	char header[] = "INSTR FORMAT OAT TAAM OBJ";
	FILE *outFile;
    outFile = fopen("obj_struct.txt", "w+");
	fprintf(outFile, "%s\n", header);
	fclose(outFile);


	// int a = 0x032600;
	char* line = "T0000001C0500000320146910178D1BA0101BC0002F20073B2FF40F102EFD9041";
	char *to = (char*) malloc(8);
	strncpy(to, line+1, 6);
	long startAddr = strtol(to, NULL, 16);
	// startAddr >>= 8;
	printf("str addr %X\n", startAddr);
	strncpy(to, line+7, 2);
	long objCodeLength = strtol(to, NULL, 16);
	// objCodeLength >>= 16;
	objCodeLength >>= 20;
	printf("obj code len %X\n", objCodeLength);
	// objCodeLength * 2 bc for ex, 28bytes / 3 bytes per word * 6 bits per hex = 28/3*6 = 28*2 :)
	for (int i = 9; i <= 9 + (objCodeLength * 2); i += 6) {
		strncpy(to, line+i, 8);
		long a = strtol(to, NULL, 16);
		//a >>= 4;
		printf("a %X\n", a);
		i += doTheThing(a);
	}

	free(to);
	return 0;
}

// Gets target address value, returns index of correct char value in associated list
int getTAAM(int bp){
	int b = 0x004000;
	int p = 0x002000;
	if ((bp & p) == p) { // pc
		return 0;
	} else if ((bp & b) == b) { // base
		return 1;
	} else { // absolute
		// technically both being 1 should fail, but if that issue arises it can be fixed in post :)
		return 2;
	}
}


int getOAT(int ni){
	int n = 0x020000;
	int i = 0x010000;
	if (((ni & i) == i) && ((ni & n) == n)) { // simple
		return 2;
	} else if ((ni & n) == n) { // indirect
		return 3;
	} else if ((ni & i) == i) { // immediate
		return 0;
	} else { // also simple (both n and i are 0)
		return 2;
	}
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


int doTheThing(long a){
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

	// pc/base/absolute (TA)
	char TAAM[][16] = {
		"pc", // 0x002000
		"base", // 0x004000
		"absolute", // ? direct? // bp == 0x000000 ?	// rubric says this is correct
		"pc_indexed",
		"base_indexed", 
		"absolute_indexed",
		""
	};

	// immediate/simple	(how used)
	char OAT[][9] = {
		"immediate", // (ni == 0x010000)
		"", // this space has to be here or it combines elements 0 and 1 :/
		"simple", // else bc ni both = 0 or 1
		"indirect", // (ni == 0x020000)
		""
	};


	int opCode = a & 0xFC0000;
	// opCode = 0x90;

	indexOfEverything = opCodeText(opCode, lengthOfLists, operations);

	printf("opcode: %X\n", opCode);	
	//printf("operat: %X\n", *operations[0]);
	//printf("operat: %X\n", 0xB8);

	// spot for finding format, should divide length of data by 8, then value will == format
	if (*isFormat2[indexOfEverything]) {
		printf("%d\n", *isFormat2[indexOfEverything]);
		format = 2;	
	} else {
		if (a & 0x001000) { // just checks e
			format = 4;
		} else {
			format = 3;
		}
	}

	int OATIndex = -1;
	int TAAMIndex = -1;

	if (format != 2){
		// OAT - immedaite/simple - how ta is used - addrmode, based on ni
		int n = 0x020000;
		int i = 0x010000;
		int ni = 0x030000;
		// TAAM - base/pc - ta itself, based on bp
		int b = 0x004000;
		int p = 0x002000;
		int bp = 0x006000;
		int x = 0x008000;
		int e = 0x001000;
	
		OATIndex = getOAT(a & ni);
		//printf("TAAM: %s\n", OAT[0]);
		TAAMIndex = getTAAM(a & bp);
		if ((OATIndex == 2) && ((a & x) == x)) { TAAMIndex += 3; } // if simple and x = 1, make it indexed
	}

	int objValue = 0x0;
	if (format == 2){
		objValue = a & 0x00FF0000;
		objValue >>= 16;
	} else if (format == 3){
		objValue = a & 0x000FF000;
		objValue >>= 12;
	} else { // format 4
		objValue = a & 0x000FFFFF;
	}


	FILE *outFile;
    outFile = fopen("obj_struct.txt", "a+");
	// fprintf(outFile, "%s\n", header);
	// fprintf(outFile, "%X\n", opCodeText(opCode, lengthOfLists, operations));
	fprintf(outFile, "%s %d %s %s %X\n", abbrev[indexOfEverything], format, OAT[OATIndex], TAAM[TAAMIndex], objValue);
	fclose(outFile);

	if (format == 4){
		return 2;
	} else {
		return 0;
	}
}