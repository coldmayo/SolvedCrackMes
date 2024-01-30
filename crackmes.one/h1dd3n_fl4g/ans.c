#include <stdio.h>
#include <string.h>

int main(void) {

    char buffUser[62];
    char buffPass[64];

    char username[20] = "r3v3rs3r";
    char password[20] = "lixachev";

    printf("[+] Ent3r L0g1n:");
    scanf("%s", buffUser);
    printf("[+] Ent3r p4$$w0rd:");
    scanf("%s", buffPass);

    if (strcmp(buffUser,(char *)&username) == 0 && strcmp(buffPass,(char *)&password) == 0) {
        printf("[!] D0 y0u th1nk 3v3ryth1ng 1s s0 s1mpl3?\n");
        return 0;
    }
    printf("[*] B4d L0g1n 0r P4$$w0rd, y0u b4d h4ck3r:(.\n");
    return 0;
}