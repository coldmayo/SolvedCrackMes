#include <stdio.h>
#include <string.h>

void main(void) {

    char pass[128];

    printf("enter password: \n");

    if (scanf("%s", pass) != 1) {
        printf("no password supplied\n");
    }

    if (strcmp(pass,"metallica") == 0) {
        printf("password is correct\n");
    } else {
        printf("password is not correct\n");
    }

}