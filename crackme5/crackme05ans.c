// This is what I assume would have been the source code of crackme05

// password: AABAAAAAAAAAAQAA (found through guess and check unfortunately)

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int check_with_mod(char * pass, int a, int b) {
    int i;
    int j = 0;
    for (i=0; i<a; i++) {
        j = j+pass[i];
    }
    return j%b == 0;
}

void fail(char * wrong) {
    printf("No %s is not correct\n", wrong);
    exit(1);
}

void succeed(char * correct) {
    printf("Yes %s is correct\n", correct);
    exit(0);
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }
    int len = strnlen(argv[1], 1000);
    if (len != 16) {
        printf("p");
        fail(argv[1]);
    }
    if (argv[1][2] != 'B') {
        printf("q");
        fail(argv[1]);
    }
    if (argv[1][13] != 'Q') {
        printf("s");
        fail(argv[1]);
    }
    if (check_with_mod(argv[1],4,3) == 0) {
        fail(argv[1]);
    }
    if (check_with_mod(argv[1]+4,4,4) == 0) {
        fail(argv[1]);
    }
    if (check_with_mod(argv[1]+8,4,5) == 0) {
        fail(argv[1]);
    }
    if (check_with_mod(argv[1]+12,4,4) == 0) {
        fail(argv[1]);
    }
    succeed(argv[1]);

}