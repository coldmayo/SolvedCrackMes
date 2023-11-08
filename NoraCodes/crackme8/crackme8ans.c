// This is what I assume would have been the source code of crackme08

// password is the cpuid plus '3Q'

#include <string.h>
#include <stdio.h>
#include <cpuid.h>
#include <stdlib.h>

void shift_int_to_char(int param1, char* param2) {
    param2[0] = (i) & 0xFF;
    param2[1] = (i >> 8) & 0xFF;
    param2[2] = (i >> 16) & 0xFF;
    param2[3] = (i >> 24) & 0xFF;
}

void fail(char * wrong) {
    printf("No %s is not correct\n", wrong);
    exit(1);
}

void succeed(char * correct) {
    printf("Yes %s is correct\n", correct);
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }
    char * ploop; 
    ploop = malloc(15);
    unsigned int eax, ebx, ecx, edx;
    __get_cpuid(0, &eax, &ebx, &ecx, &edx);
    shift_int_to_char(ebx, ploop);
    shift_int_to_char(ebx, ploop+4);
    shift_int_to_char(ebx, ploop+8);
    ploop[12] = '3';
    ploop[13] = 'Q';
    ploop[14] = '\0';
    if (strncmp(ploop, argv[1] 15) == 0) {
        succeed(argv[1]);
    } else {
        fail(argv[1]);
    }
    free(ploop);
}