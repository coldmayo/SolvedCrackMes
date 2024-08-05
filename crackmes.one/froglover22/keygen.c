#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * slice_str(char * str, char * buffer, int start, int end) {
    int j = 0;
    for (int i = start; i <= end && str[i] != '\0'; ++i) {
        buffer[j++] = str[i];
    }
    buffer[j] = '\0';
    return buffer;
}

int find_int(int mask) {
    char string[80];

    char chunks[5][80] = {
        "`fmj%'Fjkbwdqv$%Youve fopka%qm`%flag.\"",
        "kbwdqv$%Youve fopka%qm`%flag.\"",
        "Youve fopka%qm`%flag.\"",
        "pka%qm`%flag.\"",
        "flag.\"",
    };

    for (int i = 0; i < 5; i++) {
        if ((mask & (1 << i)) != 0) {
            for (int j = 0; j < 8 && chunks[i][j] != '\0'; j++) {
                chunks[i][j] = chunks[i][j] ^ 5;
            }
        }
    }

    snprintf(string, sizeof(string), "%s%s%s%s%s", chunks[0], chunks[1], chunks[2], chunks[3], chunks[4]);

    //printf("%s\n", string);

	char buffer[6];
	//printf("%s\n", slice_str(string, buffer, 0, 4));
	if (strcmp(slice_str(string, buffer, 0, 4), "echo ") == 0) {
		if (strcmp(string, "echo \"Cokbwdqv$%Youve fopka%qm`%flag.\"ngrats! Youve fopka%qm`%flag.\"Youve fopka") == 0) {
			printf("%s\n", string);
			return 1;
		}
	}

    return 0;
}

int main() {
	int res;
	for (int i = 0; i < 200; i++) {
		res = find_int(i);
		if (res == 1) {
			printf("%d\n", i);
		}
	}
	printf("done");
	return 0;
}
