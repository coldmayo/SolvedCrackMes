#include <stdio.h>
#include <stdlib.h>

int main() {
	srand(0x7bf);
	char plebus[] = "gssw#tpc\0";
	int random;
	char * pass = malloc(10 * sizeof(char));
	for (int i = 0; i < 9; i++) {
		random = rand();
		plebus[i] -= (char)((random % 5) + '\x01');
		pass[i] = plebus[i];
	}
	printf("%s\n", pass);
	free(pass);
	return 0;
}
