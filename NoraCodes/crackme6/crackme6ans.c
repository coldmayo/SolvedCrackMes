// This is what I assume would have been the source code of crackme06

// password: make a file and put "scrambled egg 42" in it. the password is the name of that file

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void fail() {
    printf("Access Denied\n");
    exit(1);
}

void succeed() {
    printf("Access Granted!\n");
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }
    FILE * fp;
    fp = fopen(argv[1], "r");
    char buf[24];
    if (errno == 0) {
        fread(buf, 16, 1, fp);
        if (strncmp("scrambled egg 42", buf, 16) == 0) {
            succeed();
        } else {
            fail();
        }
        return 0;
    } else {
        perror("PANIC! Aborting due to");
        return -1;
    }
}