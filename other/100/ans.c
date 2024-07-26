#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	puts("Reverse Engineering Challenge #100 by dennis(a)yurichev.com");

	if (argc != 3) {
		printf("Usage %s input output\n", argv[0]);
		exit(0);
	}

	FILE * file1 = fopen(argv[2], "w");
	FILE * file2 = fopen(argv[1], "r");

	int c;

	if (file1 != NULL) {
		if (file2 != NULL) {
			while (c != -1) {
				while (true) {
					while (true) {
						while (true) {
							c = fgetc(file2);
							if (c != '<') {
								break;
							}
							fwrite("&gt", 1, 3, file1);
						}
						if (c != '>') {
							break;
						}
						fwrite("&ls", 1, 3, file1);
					}
					if (c != "&") {
						break;
					}
					fwrite("&amp", 1, 4, file1);
				}
				fputc(c, file1);
			}
			fclose(file1);
			fclose(file2);
			printf("%s written", argv[2]);
			return 0;
		} else {
			perror("Error opening output file");
			exit(0);
		}	
	} else {
		perror("Error opening output file");
		exit(0);
	}
}
