#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool check_key(char * key) {
	if (key[0] + strlen(key) == 0x52) {
		if (key[2] == '0') {
			if (key[3] == key[7]) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

char * gen_key() {
    char * chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    char * ret = malloc(10 * sizeof(char));
    int i = 0;
    int sel;

    while (i < 9) {
        if (i == 4) {
            ret[i] = '-';
        } else {
            sel = rand() % strlen(chars);
            ret[i] = chars[sel];
        }
        i++;
    }
    ret[9] = '\0';
    return ret;
}

int main() {

	char * key;

	while (true) {
    	key = gen_key();
		printf("Key: %s\n", key);
		if (check_key(key)) {
			printf("This one works!\n");
			return 0;
		}
	}
}
