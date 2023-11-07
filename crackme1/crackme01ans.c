// This is what I assume would have been the source code of crackme01

// the password was: password1

#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument");
        return 1;
    }

    char * password = "password1";

    if (strcmp(password, argv[1]) == 0) {
        printf("Yes %s is correct\n", argv[1]);
        return 0;
    } else {
        printf("No %s is not correct\n", argv[1]);
        return -1;
    }

}