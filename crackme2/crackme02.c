// This is what I assume would have been the source code of crackme02

// the password was: ""

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }

    if (argv[1][0] == '\0') {
        printf("Yes %s is correct", argv[1]);
        return 0;
    }

    printf("No %s is not correct", argv[1]);
    return -1;

}