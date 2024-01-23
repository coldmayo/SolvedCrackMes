// reconstruction of e12 executable

#include <utime.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    struct stat buf;
    struct utimbuf ubuf;

    if (argc != 2) {
        printf("Usage: <filename>\n");
        exit(0);
    }

    if (-1 < lstat(argv[1], &buf)) {
        ubuf.actime = buf.st_atim.tv_sec;
        ubuf.modtime = time((time_t *)0x0);
        if (-1 < utime(argv[1], &ubuf)) {
            return 0;
        }

        printf("error #2!\n");
        exit(0);

    }

    printf("error #1!\n");
    exit(0);

}