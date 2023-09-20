#include <stdio.h>
#include <stdlib.h>

// Prototypes
int opCode(int value);


// Main function
int main() {

	char header[25] = "INSTR FORMAT OAT TAAM OBJ";

	//FILE *outFile;
	//outFile = fopen("obj_structure.txt", "w+");

	int a = 0x032600;
	int opCode = a & 0xFC0000;
//	int maskFirstTwo = 0xFF0000;
//	int maskOpCode = 0xFC0000;

//	int firstTwo = a & maskFirstTwo;
//	int opCode = firstTwo & maskOpCode;
   
	printf("%X", opCode);	

	FILE *outFile;
        outFile = fopen("obj_structure.txt", "w+");
	//fprintf(outFile, 

	return 0;
}


