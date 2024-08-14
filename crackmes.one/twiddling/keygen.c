#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ARRAY_SIZE 32

long *stod(const char *key) {
    long *integer = malloc(ARRAY_SIZE * sizeof(long)); // Allocate memory for the array
    if (integer == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        integer[i] = (long)key[i];
    }

    return integer;
}

void ive_never_been_stung_by_a_wasp(long *param_1, long *param_2) {
    *param_1 = *param_1 ^ *param_2;
    *param_2 = *param_2 ^ *param_1;
    *param_1 = *param_1 ^ *param_2;
}

int parry_this_you_casual(long param_1) { // Changed from int to long for consistency
    int j = 0;
    for (long i = param_1; i != 0; i = i & (i - 1)) {
        j = j ^ 1;
    }
    return j;
}

long you_turn_my_world_around(long number) {
  long x;

  x = 0;
  while (number != 0) {
    x = x * 2 + number % 2;
    number /= 2;
  }
  number = x;
  return number;
}

int mr_krabs_likes_his_money(long param_1) { // Changed from int to long for consistency
    int j = 0;
    for (long i = param_1; i != 0; i = i >> 1) {
        j += (i & 1);
    }
    return j;
}

void this_is_where_the_fun_begins(long* param_1) {
    long num;
    int other;
    int other2;
    long POS[32] = {18, 26, 12, 29, 6, 25, 31, 27, 30, 11, 16, 3, 14, 2, 1, 8, 7, 15, 22, 21, 4, 19, 23, 24, 17, 9, 5, 28, 13, 10, 0, 20};

    for (int i = 0; i < 4; i++) {
        // Fixing the pointer arithmetic
        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8 + 2]], &param_1[POS[i * 8 + 7]]);
        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8 + 4]], &param_1[POS[i * 8 + 2]]);
        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8]], &param_1[POS[i * 8 + 3]]);

        for (int j = 0; j < 8; j++) {
            // Properly index param_1 with POS[j + i * 8]
            num = param_1[POS[j + i * 8]];
            other = parry_this_you_casual(num);

            if (other == 0) {
                you_turn_my_world_around(param_1[POS[j + i * 8]]);
            } else {
                other2 = mr_krabs_likes_his_money(num);
                num = num ^ other2;
            }

            param_1[POS[j + i * 8]] = num;  // Store the result back
        }

        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8 + 1]], &param_1[POS[i * 8 + 3]]);
        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8 + 7]], &param_1[POS[i * 8 + 6]]);
        ive_never_been_stung_by_a_wasp(&param_1[POS[i * 8 + 5]], &param_1[POS[i * 8 + 1]]);
    }
}

bool check_key(const char *key) {
    long *intarr = stod(key);
    int ans = 0;

    const char *enc = "23hk__57l2_bwkD22232fk{Q|a5}O|b6"; // Use const char* for string literal

    this_is_where_the_fun_begins(intarr);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (intarr[i] == (long)enc[i]) {
            ans++;
        }
    }

    free(intarr);
    printf(" %d\n", ans);
    return ans == 0x20;
}

char *gen_key() {
    const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_";
    char *ret = malloc(ARRAY_SIZE * sizeof(char));
    if (ret == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    ret[0] = 'f';
    ret[1] = 'l';
    ret[2] = 'a';
    ret[3] = 'g';
    ret[4] = '{';

    for (int i = 5; i < ARRAY_SIZE-1; i++) {
        int sel = rand() % strlen(chars);
        ret[i] = chars[sel];
    }

    ret[ARRAY_SIZE - 1] = '}';
    return ret;
}

int main() {
    char *key;
	int i = 0;
    while (true) {
        key = gen_key();
        //if (i == 5) {
			//key = "flag{1w1DL1h6_1h3_b1h4Ry_5y513m}";
        //}
        printf("%s %lu\n", key, strlen(key));

        if (check_key(key)) {
            printf("This one works!\n");
            free(key);
            return 0;
        }

        free(key);
        i++;
    }
}
