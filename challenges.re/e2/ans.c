#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int peepee(char * one, char * two) {
    FILE * file;
    FILE * file2;
    int retVal;
    long len1;
    long len2;
    char buffer1[100];
    char buffer2[100];
    size_t uno;
    size_t dos;

    file = fopen(one, "rb");
    if (file == (FILE *)0x0) {
        retVal = 1;
    } else {
        file2 = fopen(two, "rb");
        if (file2 == (FILE *)0x0) {
            retVal = 1;
        } else {
            fseek(file, 0, 2);
            fseek(file2, 0, 2);
            len1 = ftell(file);
            len2 = ftell(file2);
            if (len1 != len2) {
                retVal = 1;
            } else {
                rewind(file);
                rewind(file2);
                while (true) {
                    uno = fread(buffer1, 1, 1, file);
                    dos = fread(buffer2, 1, 1, file2);
                    if ((uno | dos) == 0) {
                        break;
                    }
                    if ((uno != dos) || memcmp(buffer1, buffer1, uno) != 0) {
                        retVal = 0;
                        break;
                    }
                }
            }
            fclose(file2);
        }
        fclose(file);
    }

    return retVal;
}

int main(int argv, char* argc[]) {
    
    int retVal;

    if (argv < 3) {
        retVal = 1;
    } else {
        retVal = peepee(argc[1], argc[2]);
    }

    return retVal;
}