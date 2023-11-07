// This is what I assume would have been the source code of crackme04

// password: nnnnnnnnnnnnnnnp

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }

    int i;
    int j;
    bool plebus;

    for (j=0; argv[1][j] != '\0'; j++) {
        i = i + argv[1][j];
    }

    plebus = false;

    if (j != 16 && i == 1762) {
        plebus = true;
    }

    if (plebus) {
        printf("Yes %s is correct\n", argv[1]);
        return 0;
    }
    printf("No %s is not correct\n", argv[1]);
    return 1;
}