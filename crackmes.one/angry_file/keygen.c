#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool check_key(char * key) {
	if (key[0]+key[1] == 0xA6 && (key[3]^key[5]) == 0x50 && key[2]+key[6] == 0xD7 && key[4]/key[7] == 0x01  && key[11]*key[4] == 0x3c0f && key[8]-key[2] == 0x09 && key[9] + key[3] == 0x97 && (key[11] & key[10]) == 0x64 && key[11]+key[0] == 0xf3 && key[1]+key[10] == 0x94 && key[9]%key[2] == 0x33 && (key[3]^key[8]) == 0x16 && (key[4] | key[7]) == 0x7f && key[6]+key[5] == 0xa2) {
		return true;
	}
	return false;
}

char * gen_key() {
	const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_!?";
    char *ret = malloc(12 * sizeof(char));

    for (int i = 0; i < 12; i++) {
		int sel = rand() % strlen(chars);
        ret[i] = chars[sel];
    }
	// after some investigation I could deduce:
    ret[0] = 'v';
    ret[1] = '0';
    ret[2] = 'i';
    ret[3] = 'd';
    ret[10] = 'd';
    ret[11] = '}';
    ret[4] = '{';
    ret[9] = '3';
    return ret;
}

int main() {

	char * key;

	while (true) {
		key = gen_key();
		printf("%s\n", key);
		if (check_key(key)) {
			printf("This one works\n");
			return 0;
		}
	}
}
