#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool check(char * key) {
	char * string = "dG46rskj8-457(~:";
	char * param_1 = key;
	if (strlen(key) == 0x40) {
		if ((*param_1 ^ string[0] ^ param_1[1] ^ string[1] ^ param_1[2] ^ string[2] ^ param_1[3] ^ string[3] ^ param_1[4] ^ string[4] ^ param_1[5] ^ string[5] ^ param_1[6] ^ string[6] ^ param_1[7] ^ string[7] ^ param_1[8] ^ string[8] ^ param_1[9] ^ string[9] ^param_1[10] ^ string[10] ^ param_1[0xb] ^ string[0xb] ^ param_1[0xc] ^ string[0xc] ^ param_1[0xd] ^ string[0xd] ^ param_1[0xe] ^ string[0xe]) == (param_1[0xf] ^ string[0xf])) {
			printf("\nSo far so good...\n\n");
			int var2 = 0;
			int var1 = (param_1[0xb] ^ string[0xb]) * 0x88 + 0x13a;
			while (var2 != 0x10) {
				var1 -= param_1[var2+0x10];
				var2++;
			}

			if (var1 == 0) {
				printf("\nYipee\n\n");
				return true;
			}
		}
	} else {
		printf("len error");
	}
	return false;
}

char * keygen() {
	char * chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    char * ret = malloc(0x41 * sizeof(char));
    int i = 0;
    int sel;
    char * rest = "YYYYYYYYAAAAAAAABBBBBBBBZZZZZZZZ";

    while (i < 0x40) {
		if (i >= 0x20) {
			i++;
        } else {
            sel = rand() % strlen(chars);
            ret[i] = chars[sel];
			i++;
        }
    }

	strcat(ret, rest);
    
    return ret;
}

int main() {
	char * key;

	while (true) {

		key = keygen();
		printf("%s\n", key);
		if (check(key) == true) {
			printf("This works!");
			return 0;
		}
	}
}
