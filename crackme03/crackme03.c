// This is what I assume would have been the source code of crackme03

// the password was: nDoEiA

#include <string.h>
#include <stdio.h>

int check_pw(char* pass, char*inp, char*mask) {
    int i = 0;
    while (pass[i] != '\0' && inp[i] != '\0') {
        if (mask[i]+pass[i] != inp[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }

    char pass[6+1] = "lAmbDa";
    char mask[6+1] = {2,3,2,3,5};

    if (strlen(argv[1]) == 6) {
        if (check_pw(pass, argv[1], mask) == 1) {
            printf("Yes %s is correct\n", argv[1]);
            return 0;
        }
    }
    printf("No %s is not correct\n", argv[1]);
    return -1;
}