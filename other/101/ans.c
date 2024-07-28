#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void extract() {
	FILE * f_vol1 = fopen("vol1", "rb");
	if (f_vol1 == NULL) {
		puts("Can't open vol1 file");
		exit(0);
	}
	
	FILE * f_vol2 = fopen("vol2", "rb");
	if (f_vol2 == NULL) {
		puts("Can't open vol2 file");
		exit(0);
	}
	
	FILE * f_vol3 = fopen("vol3", "rb");
	if (f_vol3 == NULL) {
		puts("Can't open vol3 file");
		exit(0);
	}
	
	FILE * f_vol4 = fopen("vol4", "rb");
	if (f_vol4 == NULL) {
		puts("Can't open vol4 file");
		exit(0);
	}

	FILE * f_out = fopen("out.bin", "wb");
	if (f_out == NULL) {
		puts("Can't open out.bin file");
		exit(0);
	}

	int i = 0;
	
	while (true) {
    	char buffer[100];
		size_t n = fread(buffer, 1, 1, f_vol1);
		if (n != 1) {
			puts("Extract successful, out.bin written");
			fclose(f_out);
			fclose(f_vol1);
			fclose(f_vol2);
			fclose(f_vol3);
			fclose(f_vol4);
			return;
		}

		char buffer1[100];
		n = fread(buffer1, 1, 1, f_vol2);
		if (n != 1) {
			puts("Error reading vol2 file");
			exit(0);
		}

		char buffer2[100];
		n = fread(buffer2, 1, 1, f_vol3);
		if (n != 1) {
			puts("Error reading vol3 file");
			exit(0);
		}
		
		char buffer3[100];
		n = fread(buffer3, 1, 1, f_vol4);
		if (n != 1) {
			puts("Error reading vol4 file");
			exit(0);
		}

		fwrite(buffer, 1, 1, f_out);
		fwrite(buffer1, 1, 1, f_out);
		fwrite(buffer2, 1, 1, f_out);
		fwrite(buffer3, 1, 1, f_out);
		i++;
	}
}

void check() {
	FILE * f_vol1 = fopen("vol1", "rb");
	if (f_vol1 == NULL) {
		puts("Can't open vol1 file");
		exit(0);
	}
	
	FILE * f_vol2 = fopen("vol2", "rb");
	if (f_vol2 == NULL) {
		puts("Can't open vol2 file");
		exit(0);
	}
	
	FILE * f_vol3 = fopen("vol3", "rb");
	if (f_vol3 == NULL) {
		puts("Can't open vol3 file");
		exit(0);
	}
	
	FILE * f_vol4 = fopen("vol4", "rb");
	if (f_vol4 == NULL) {
		puts("Can't open vol4 file");
		exit(0);
	}

	FILE * f_vol5 = fopen("vol5", "rb");
	if (f_vol5 == NULL) {
		puts("Can't open vol5 file");
		exit(0);
	}

	int i = 0;

	while (true) {
		char buffer[100];
		size_t n = fread(buffer, 1, 1, f_vol1);
		if (n != 1) {
			puts("Check successful");
			fclose(f_vol5);
			fclose(f_vol1);
			fclose(f_vol2);
			fclose(f_vol3);
			fclose(f_vol4);
			return;
		}

		char buffer1[100];
		n = fread(buffer1, 1, 1, f_vol2);
		if (n != 1) {
			puts("Error reading vol2 file");
			exit(0);
		}

		char buffer2[100];
		n = fread(buffer2, 1, 1, f_vol3);
		if (n != 1) {
			puts("Error reading vol3 file");
			exit(0);
		}
		
		char buffer3[100];
		n = fread(buffer3, 1, 1, f_vol4);
		if (n != 1) {
			puts("Error reading vol4 file");
			exit(0);
		}

		char buffer4[100];
		n = fread(buffer4, 1, 1, f_vol5);
		if (n != 1) {
			puts("Error reading vol5 file");
			exit(0);
		}
		i++;
	}
}
int main(int argc, char * argv[]) {
	if (argc != 2) {
		printf("Usage %d [command]\n", argc);
		exit(0);
	}

	if (strcmp(argv[1], "extract") == 0) {
		extract();
		exit(0);
	}
	if (strcmp(argv[1], "check") == 0) {
		check();
		exit(0);
	}
	printf("Unknown Command: %s\n", argv[1]);
	return 0;
}


