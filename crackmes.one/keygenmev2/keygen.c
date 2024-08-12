#include <stdio.h>

void make_serial(char * user, char * pass) {

}

int main() {
	const char * user = "DZONERZY";

	char serial[29+1];
	memset(serial, 0, sizeof(serial));

	make_serial(user, serial);
	
	printf("key is: %s", serial);
}
