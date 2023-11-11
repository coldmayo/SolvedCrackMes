// This is what I assume to be the source code of the keyg3nme executable

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

bool validate_key(int pass) {
    return pass % 1223 == 0;
}

int main (int argc, char** argv) {
    int pass;
    printf("Enter your key: ");
    scanf("%d", &pass);
    int corr = validate_key(pass);
    if (corr) {
        printf("Good job mate, now go keygen me.\n");
        return 0;
    }
    printf("nope\n");
    return 1;
}