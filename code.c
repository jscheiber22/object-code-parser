// James Scheiber
// RedID: 825617332


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Prototypes
int opCodeIndex(int opCode, int lengthOfLists, int operations[][1]);
int getTAAM(int bp);
int getOAT(int ni, int format);


// Main function
int main(int argc, char *argv[]) {

	// kill the program if a file is not provided
	if (argc < 2){
		printf("No filename provided in command execution. Exiting.\n");
		return -1;
	}

	int lengthOfLists = 59;
	int indexOfEverything, format;
	unsigned int opCode;

	// Lists of data to help assign values
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
		"    "
	};
	// immediate/simple	(how used)
	char OAT[][9] = {
		"immediate", // (ni == 0x010000)
		"", // this space has to be here or it combines elements 0 and 1 :/
		"simple", // else bc ni both = 0 or 1
		"indirect", // (ni == 0x020000)
		"    "
	};


	// add just the header info to the file
	char header[] = "INSTR FORMAT OAT TAAM OBJ";
	FILE *outFile;
    outFile = fopen("obj_struct.txt", "w+");
	fprintf(outFile, "%s\n", header);
	fclose(outFile);


	FILE *inFile;
	char line[100];
	inFile = fopen(argv[1], "r");
	// iterate through given file line by line
	while (fgets(line, sizeof(line), inFile)){
		// Only check text lines for instructions
		if (line[0] == *"T"){
			char *to = (char*) malloc(8);
			strncpy(to, line+1, 6);
			long startAddr = strtol(to, NULL, 16); // starting address
			strncpy(to, line+7, 2);
			long objCodeLength = strtol(to, NULL, 16); // length of line of object code
			objCodeLength >>= 16; // bit shift to keep proper length

			// objCodeLength * 2 bc for ex, 28bytes / 3 bytes per word * 6 bits per hex = 28/3*6 = 28*2 :)
			// The main for loop that moves through each line instruction by instruction
			for (int i = 9; i <= 9 + (objCodeLength * 2); i += 6) {
				// A bit of logic to test if the remainder of the line has enough hex values to grab 8 more.
				// if not, it just grabs the remaining values
				if (i + 8 <= 9 + (objCodeLength * 2)){
					strncpy(to, line+i, 8);
				} else {
					strcpy(to, &line[i]);
				}
				long instruction = strtol(to, NULL, 16);
				if (instruction == 0){
					break; 
				}

				// calculate the length of the hex value for more accurate calculations
				char hex_string[10];
    			snprintf(hex_string, sizeof(hex_string), "%X", instruction);
				int num_hex_digits = strlen(hex_string);

				// take care of some edge cases like having length of < 8  and find the opcode
				if (instruction <= 0xFFFF){
					opCode = instruction & 0xFC00;
					opCode >>= 8;
				} else if (num_hex_digits == 6){
					opCode = instruction & 0xFC0000;
					opCode >>= 16;
				} else {
					opCode = instruction & 0xFC000000;
					opCode >>= 24;
				}

				// this variable is used to match the corresponding value within the above lists
				indexOfEverything = opCodeIndex(opCode, lengthOfLists, operations);

				// code for finding format based on given list and the value of e
				if (*isFormat2[indexOfEverything]) {
					format = 2;
					if (num_hex_digits > 4){
						instruction >>= (num_hex_digits - 4) * 4; // remove extra digits
					}
				} else {
					if (num_hex_digits == 6){ // must be format 3 if < 8 digits
							format = 3;
					} else if (instruction & 0x00100000) { // 0x00100000 = e for format 4
						format = 4;
					} else {
						format = 3;
						instruction >>= 8;
					}
				}
				

				// defualts to blank space index of list for formatting
				int OATIndex = 4;
				int TAAMIndex = 6;

				if (format == 3){
					// OAT - immedaite/simple - how ta is used - addrmode, based on ni
					int ni = 0x030000;
					// TAAM - base/pc - ta itself, based on bp
					int bp = 0x006000;
					int x = 0x008000;
				
					OATIndex = getOAT(instruction & ni, format);
					TAAMIndex = getTAAM(instruction & bp);
					if ((OATIndex == 2) && ((instruction & x) == x)) { TAAMIndex += 3; } // if simple and x = 1, make it indexed
				} else if (format == 4){
					int ni = 0x03000000;
					OATIndex = getOAT(instruction & ni, format);
					TAAMIndex = 2; // always absolute for format 4
				}


				// Writes line of data to output file depending on format so that preceeding zeros are correct in number
				if (format == 4){
					FILE *outFile;
					outFile = fopen("obj_struct.txt", "a+");
					fprintf(outFile, "%s    %d      %s    %s    %08X\n", abbrev[indexOfEverything], format, OAT[OATIndex], TAAM[TAAMIndex], instruction) ;
					fclose(outFile);
					i += 2;
				} else if (format == 3) {
					FILE *outFile;
					outFile = fopen("obj_struct.txt", "a+");
					fprintf(outFile, "%s    %d      %s    %s    %06X\n", abbrev[indexOfEverything], format, OAT[OATIndex], TAAM[TAAMIndex], instruction) ;
					fclose(outFile);
					i += 0;
				} else { // format 2 by default
					FILE *outFile;
					outFile = fopen("obj_struct.txt", "a+");
					fprintf(outFile, "%s    %d      %s    %s    %04X\n", abbrev[indexOfEverything], format, OAT[OATIndex], TAAM[TAAMIndex], instruction) ;
					fclose(outFile);
					i -= 2;
				}
			}

			// free allocated resources
			free(to);
		}
	}

	return 0;
}


// Gets TAAM value, returns index of correct char value in associated list
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

// Gets OAT value, returns index of correct char value in associated list
int getOAT(int ni, int format){
	int n, i, both;
	// set variables for readability, different lengths for each format type
	if (format == 3){
		n = 0x020000;
		i = 0x010000;
		both = 0x030000; // if both n and i are set to 1
	} else {
		n = 0x02000000;
		i = 0x01000000;
		both = 0x03000000;
	}
	
	if (ni == both) { // simple
		return 2;
	} else if (ni == n) { // indirect
		return 3;
	} else if (ni == i) { // immediate
		return 0;
	} else { // also simple (both n and i are 0)
		return 2;
	}
}


// Gets operation code value, returns index of correct char value in associated list
int opCodeIndex(int opCode, int lengthOfLists, int operations[][1]) {
	for (int i = 0; i < lengthOfLists - 1; i++) {
		if (opCode == *operations[i]) {
			return i;
		}
  	}	

	return 0x0; // 0 for fail

}