#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char * slice(const char * str, int start, int len) {
	char* result = (char*)malloc((len + 1) * sizeof(char));
	strncpy(result, str + start, len);
	result[len] = '\0';
	return result;
}

uint32_t bullshit(uint32_t part_key) {
	for (int i = 0; i < 32; ++i) {
		part_key = (part_key >> 12) | ((part_key & 0xff) ^ 0x15);
	}
	return part_key;
}

bool check(char *key) {
	// check beginning indexes 0 through 3
	int len = strlen(key) + 1;
	char leng0[4] = {(char)len, (char)len, (char)len, (char)len};
	char * sliced = slice(key, 0, 3);
	uint32_t beg = *(uint32_t *)sliced;
	//printf("%s\n", sliced);
	uint32_t leng = *(uint32_t *)leng0;
	beg ^= leng;
	beg = __builtin_bswap32(beg);
	beg = bullshit(beg);

	if (beg == 0xef) {
		
		sliced = slice(key, 6, 4);
		uint32_t mid = *(uint32_t *)sliced;
				
		sliced = slice(key, 22, 5);
		sliced+='\n';
		sliced+='\0';
		uint32_t end = *(uint32_t *)sliced;
		

		end = __builtin_bswap32(end);
		mid ^= end;
		mid = __builtin_bswap32(mid);
		mid = bullshit(mid);

		mid ^= leng;
		mid = __builtin_bswap32(mid);
		mid = bullshit(mid);

		if (mid == 0x6f) {
			return true;
		}
	}
	
	return false;
}

char * gen_key() {
	const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_ ";
    char * ret = malloc(35);

    strcpy(ret, "____all those wh____nder are los__");

    for (int i = 0; i < 34; i++) {
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 16 || i == 17 || i == 18 || i == 19 || i == 32 || i == 33) {
        	int sel = rand() % strlen(chars);
        	ret[i] = chars[sel];
		}
	}
	return ret;
}

int main() {

	char * key;

	while (true) {
		key = gen_key();
		//key = "Not all those who wander are lost;";
		printf("%s\n", key);

		if (check(key)) {
			printf("This one works");
			return 0;
		}
	}
}
