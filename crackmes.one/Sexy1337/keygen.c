#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char * string = "sexy1337";
	int num = 4294967210;
	char buff[56];
	
	for (int i = 0; strlen(string) > i; i++) {
		buff[i] = string[i] + '\x03';
	}

	printf("%s\n", buff);
}
