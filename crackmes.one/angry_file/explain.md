First, I'll look at the main() function

```bash
mov qword [s], 0
mov qword [var_28h], 0
mov qword [var_22h], 0
mov qword [var_1ah], 0
lea rax, [s]
mov rsi, rax
lea rax, [0x00002004]   # "%s"
mov rdi, rax
mov eax, 0
call sym.imp.scanf
lea rax, [s]
mov rdi, rax
call sym.imp.strlen
cmp rax, 0xc
jne 0x132b
```

It seems this clip of code uses scanf to take user input, when that is done it measures its length. If it is not equal to 12 then a jump occurs which ends the program. Otherwise:
```bash
movzx eax, byte [s]
movsx edx, al
movzx eax, byte [var_2fh]
movsx eax, al
add eax, edx
cmp eax, 0xa6
jne 0x132b
```

It seems that the user input (s) is zero extended and put into eax, then moved into edx.
I was confused as to what var_2fh was so I checked Ghidra and it seems to be s + 0x1.
So, s[1] is put into eax, and then added to edx. Then eax is compared with 0xa6 (166) and if it is not equal to that the program ends.
Possible s[0] and s[1] contenders are 'A' and 'e'.
s[0] + s[1] = 0xa6
Otherwise:

```bash
movzx edx, byte [s + 0x5]
movzx eax, byte [s + 0x3]
xor eax, edx
cmp al, 0x50
jne 0x132b
```

Seems this piece of code takes s[5] and s[3] and xor's them. If this ends up equaling 0x50 (80) then the program continues
s[3]^s[5] = 0x50

```bash
movzx eax, byte [s + 0x2]
movsx edx, al
movzx eax, byte [s + 0x6]
movsx eax, al
add eax, edx
cmp eax, 0xd7
jne 0x132b
```

This code takes s[2] and s[6], adds them and then sees if this adds up to 0xd7 (215) then it continues.
s[2]+s[6] = 0xd7

```bash
movzx eax, byte [s + 0x4]
movsx eax, al
movzx edx, byte [s + 0x7]
movsx ecx, dl
cdq
idiv ecx
cmp eax, 0x1
jne 0x132b
```

Seems to divide s[7] by s[4] and checks if the quotent is 1. If it is the program continues:
s[4]/s[7] = 0x01

```bash
movzx eax, byte [s + 0x11]
movsx edx, al
movzx eax, byte [s + 0x4]
movsx eax, al
imul eax, edx
cmp edx, 0x3c0f
jne 0x132b
```

I think this code takes s[11] and s[4] and multplies them.
s[11]*s[4] = 0x3c0f
```bash
movzx eax, byte [s + 0x8]
movsx edx, al
movzx eax, byte [s + 0x2]
movsx eax, al
sub edx, eax
cmp edx, 0x9
jne 0x132b
```

It seems that s[8] - s[2] has to equal 9.
s[8] - s[2] = 0x9
```bash
movzx eax, byte [s + 0x9]
movsx edx, al
movzx eax, byte [s + 0x3]
movsx eax, al
add eax, edx
cmp eax, 0x97
jne 0x132b
```

s[9] + s[3] = 0x97

```bash
movzx edx, byte [s + 0x10]
movzx eax, byte [s + 0x11]
and eax, edx
cmp al, 0x64
jne 0x132b
```
s[10] & s[11] = 0x64

```bash
movzx eax, byte [s+0x11]
movsx edx, al
movzx eax, byte [s]
movsx eax, al
add eax, edx
cmp eax, 0xf3
jne 0x132b
```
s[11]+s[0] = 0xf3

I am lazy, the rest are:
- s[1]+s[10] = 0x94
- s[9] % s[2] = 0x33
- s[8]^s[3] = 0x16
- s[4] | s[7] = 0x7f
- s[6]+s[5] = 0xa2

I was able to make a keygen:

```bash
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool check_key(char * key) {
	if (key[0]+key[1] == 0xA6 && (key[3]^key[5]) == 0x50 && key[2]+key[6] == 0xD7 && key[4]/key[7] == 0x01  && key[11]*key[4] == 0x3c0f && key[8]-key[2] == 0x09 && key[9] + key[3] == 0x97 && (key[11] & key[10]) == 0x64 && key[11]+key[0] == 0xf3 && key[1]+key[10] == 0x94 && key[9]%key[2] == 0x33 && (key[3]^key[8]) == 0x16 && (key[4] | key[7]) == 0x7f && key[6]+key[5] == 0xa2) {
		return true;
	}
	return false;
}

char * gen_key() {
	const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_!?";
    char *ret = malloc(12 * sizeof(char));

    for (int i = 0; i < 12; i++) {
		int sel = rand() % strlen(chars);
        ret[i] = chars[sel];
    }
	// after some investigation I deduced:
    ret[0] = 'v';
    ret[1] = '0';
    ret[2] = 'i';
    ret[3] = 'd';
    ret[10] = 'd';
    ret[11] = '}';
    ret[4] = '{';
    ret[9] = '3';
    return ret;
}

int main() {

	char * key;

	while (true) {
		key = gen_key();
		printf("%s\n", key);
		if (check_key(key)) {
			printf("This one works\n");
			return 0;
		}
	}
}

```
You should get v0id{4nFr3d} as a password which should work.
