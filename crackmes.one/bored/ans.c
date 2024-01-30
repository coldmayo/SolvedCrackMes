// key is caesarcipher

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void fun(char * param, int param2) {
    int i = 0;
    int leng;
    int var;

    while(true) {
        leng = strlen(param);
        if (leng <= i) {
            break;
        }
        if (param[i] < 'A' || ('Z' < param[i])) {
            if ('`' < param[i] && param[i]< '{') {
                var = param2 + param[i] - 0x61;
                param[i] = (char)var + (char)(var/0x1a) * (-0x1a) + 'a';
            }
        } else {
            var = param2 + param[i] - 0x41;
            param[i] = (char)var + (char)(var/0x1a) * (-0x1a) + 'A';
        }
        i += 1;
    }
    return;
}

int main(void) {

    char pass[112];
    char * one = "geiwevgmtliv";
    printf("Enter the password: ");
    scanf("%s", pass);
    fun(pass, 4);
    printf("%s\n", pass);
    if (strcmp(pass, one) == 0) {
        printf("Correct!");
        return 0;
    }
    printf("Nope\n");
    return 0;
}