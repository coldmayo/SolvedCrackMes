// This is what I assume would have been the source code of crackme07

// password: password1 as long as the time is between 5 and 6

#include <stdio.h>
#include <string.h>
#include <time.h>

void fail() {
    printf("Access Denied\n");
    exit(1);
}

void succeed() {
    printf("Access Granted!\n");
    exit(0);
}

int cur_hour() {
    time_t rawt;
    time(&rawtime);
    struct tm curr_time = localtime(&rawtime);
    return curr_time->tm_hour;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("Need exactly one argument\n");
        return 1;
    }
    if (strncmp(argv[1], "password1",9) == 0) {
        if ((cur_hour() < 5) || (cur_hour() > 6)) {
            fail();
        }
        succeed();
    } else {
        fail();
    }
}