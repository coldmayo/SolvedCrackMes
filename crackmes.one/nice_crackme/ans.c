#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

int len(char* pass){
	if(*pass) {
        return 1+len(pass+1);
    }
    else {
        return 0;
    }
}

int check_len(char* pass){
    if (len(pass)%4) {
        return 0;
    } else {
        return 1;
    }
}

int check_palindrom4(char *pass){
    if ((*pass==*(pass+3))&&(*(pass+1)==*(pass+2))) {
        return 1;
    }
    return 0;
}
int check_palindrom_every_four(char *pass){
	if(*pass == '\0') {
        return check_palindrom4(pass)&check_palindrom_every_four(pass+4);
    }
	return 1;
}

int check_all(char *pass){
	return check_len(pass)&&check_palindrom_every_four(pass);
}

void main(void) {
    char *pass=(char*)malloc(sizeof(char)*201);
    int game = 1;
    int lent;
    int check;
    while (true) {
        if (game > 5) {
            printf("Game Over");
            exit(0);
        }
        printf("Enter Password: ");
        if (fgets(pass, 201, stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            free(pass);
            exit(1);
        }
		pass[len(pass)-1]=0;
        check = check_all(pass);
        if (check != 0) {
            break;
        } else {
            printf("WRONG number of attempt: %d\n", game);
            game = game + 1;
        }
    }
    printf("CORRECT");
    free(pass);
    exit(0);
}