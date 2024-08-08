I first started by opening this executable in radare2 and it seems to be written in C++.

The first few lines of code read:
```bash
lea rax, [var_30h]
mov rdi, rax
call sym std::basic_string
mov esi, str_enter   # "Enter serial code"
mov edi, obj.std::out
call std::basic_osstream
lea rax, [var_30h]
mov rsi, rax
mov esi, obj.std::cin
call sym.basic_istream
```
It seems that the code prints out "Enter serial code" and then takes in the user input. Then we see the following:
```bash
lea rax, [var_30h]
mov rdi, rax
call std::basic_string.size()
nop
cmp rax, 7
jbe 0x4011fa
```
It seems that the user input is checked to see if it has a length greater than 7. If it doesn't then the does not occur. If this is the case then "[-] Incorrect password! " is printed and the program ends.
Otherwise:
```bash
lea rax, [var_30h]
mov rdi, rax
call sym std::basic_string.size()
nop
cmp rax, 0xb   # 11
ja 0x4011fa
mov eax, 1
jmp 0x4011ff
mov eax, 0
test al, al
je 0x4012c5
```
I think this is checking if the length of the string is greater than 11. If it is not then the program ends and prints "[-] Incorrect password! ".
Otherwise:
```bash
lea rax, [var_30h]
mov esi, 0
mov rdi, rax
call sym std::basic_string
movzx eax, byte [rax]
movsx rbx, al
call sym std::basic_string
nop
add rax, rbx
cmp rax, 0x52
```
I think it takes user_input[0] and adds the length of the string to it. If it equals 0x52 then the the code continues.
Otherwise
```bash
lea rax, [var_30h]
mov esi, 4
mov rdi, rax
call sym std::basic_string
movzx eax, byte [rax]
cmp al, 0x2d   # '-'
```
I believe now the code checks if the user_input[4] is a dash '-'

```bash
lea rax, [var_30h]
mov esi, 2
mov rdi, rax
call sym std::basic_string
movzx eax, byte [rax]
cmp al, 0x30   '0'
```
Checks if the user_input[2] is '0'
```bash
lea rax, [var_30h]
mov esi, 3
mov rdi, rax
call sym std::basic_string
movzx ebx, byte [rax]
lea rax, [var_30h]
mov esi, 7
mov rdi, rax
call sym std::basic_string
movzx eax, byte [rax]
cmp bl, al
```
Lastly, it checks if user_input[3] is equal to user_input[7]. If all of the above is true, then the program succeeds.

Now we need to make a keygen:

```bash
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool check_key(char * key) {
	if (key[0] + strlen(key) == 0x52) {
		if (key[2] == '0') {
			if (key[3] == key[7]) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

char * gen_key() {
    char * chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    char * ret = malloc(10 * sizeof(char));
    int i = 0;
    int sel;

    while (i < 9) {
        if (i == 4) {
            ret[i] = '-';
        } else {
            sel = rand() % strlen(chars);
            ret[i] = chars[sel];
        }
        i++;
    }
    ret[9] = '\0';
    return ret;
}

int main() {

	char * key;

	while (true) {
    	key = gen_key();
		printf("Key: %s\n", key);
		if (check_key(key)) {
			printf("This one works!\n");
			return 0;
		}
	}
}

```

If this is run we get: IO02-DQ2D

Which does work.
