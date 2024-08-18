First we will check out main:

```bash
push rbp
pxor xmm0, xmm0
lea rdi, str.Input:   # "Input: "
sub rsp, 0x50
movaps xmmword [rsp], xmm0
mov rbp, rsp
movaps xmmword [var_10h], xmm0
movaps xmmword [var_20h], xmm0
movaps xmmword [var_30h], xmm0
mov byte [var_40h], 0
call sym.imp.puts
```

It seems Input: is printed with the puts() function. It seems other things are saved as well.

```bash
mov rdx, qword [obj.stdin]
mov esi, 0x41
mov rdi, rbp
call sym.imp.fgets
```

fgets(user_input, 0x41, stdin) is used to take user input and save to user_input char array.

```bash
mov rdi, rbp
call sym.imp.strlen
mov rdi, rbp
mov esi, eax
call fcn.0040152b
add rsp, 0x50
pop rbp
ret
```

It seems user_input and strlen(user_input) are passed into fcn_0040152b. Then after that is run the function returns 0.

Lets check out fcn_0040152b:

```bash
cmp esi, 0x40   # esi is param_2
mov qword [0x4040a0], rdi
jne fcn.0040168a
```

fcn.0040168a is the function that tells you that you failed. So we need the length of the input to be 0x40 (64). Then the below if statement is executed:

```bash
char * string = "dG46rskj8-457(~:"
if ((*param_1 ^ string[0] ^ param_1[1] ^ string[1] ^ param_1[2] ^ string[2] ^ param_1[3] ^ string[3] ^ param_1[4] ^ string[4] ^ param_1[5] ^ string[5] ^ param_1[6 Also, xmm1 is saved into 0x4040b0] ^ string[6] ^ param_1[7] ^ string[7] ^ param_1[8] ^ string[8] ^ param_1[9] ^ string[9] ^ param_1[10] ^ string[10] ^ param_1[11] ^ string[11] ^ param_1[12] ^ string[12] ^ param_1[13] ^ string[13] ^ param_1[14] ^ string[14]) == (param_1[15] ^ string[15])) {
	...
}
```

Interesting, "dG46rskj8-457(~:" seems to be xored with the user input. If the if statement does not hold, fcn0040152b is called. Also, xmm1 is saved into 0x4040b0. xmm1 is param_1+0x10 or the string after 0x10.

```bash
int var = (param_1[11] ^ string[11]) * 0x88 + 0x13a;
int i = 0;
while (i != 0x10) {
	var -= (param_1 + 0x10)[i];
	i++;
}
char * string1 = param_1 + 0x18;
if (i == 0) {
	yay?
}
```

But this still doesn't work. What are we missing?

So, we need to cause a SIGFPE in the div. The numerator is (param_1 + 0x20)^(param_1 + 0x20) and the denominator is some big number. We need to make the numerator as small as possible but have it not reach 0. So we need 1.
All we have to do is set param_1+0x20 to param_1+0x2f to a letter (lets say Y) and then set param_1+0x30 to param_1+0x3f to that letter +1 (Z)

But you'll notice that also does not work. That is because when this function fails it enters into another one. After poking around, it seems that it jumps to fcn_00401200.

After looking, it seems there are many paths to getting things saved into eax. I will choose the method I chose below:

The easiest one is probably having the last 32 characters in the form of:

```bash
YYYYYYYYAAAAAAAABBBBBBBBZZZZZZZZ
```

Ok so we can make a keygen I think:

```bash
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool check(char * key) {
	char * string = "dG46rskj8-457(~:";
	char * param_1 = key;
	if (strlen(key) == 0x40) {
		if ((*param_1 ^ string[0] ^ param_1[1] ^ string[1] ^ param_1[2] ^ string[2] ^ param_1[3] ^ string[3] ^ param_1[4] ^ string[4] ^ param_1[5] ^ string[5] ^ param_1[6] ^ string[6] ^ param_1[7] ^ string[7] ^ param_1[8] ^ string[8] ^ param_1[9] ^ string[9] ^param_1[10] ^ string[10] ^ param_1[0xb] ^ string[0xb] ^ param_1[0xc] ^ string[0xc] ^ param_1[0xd] ^ string[0xd] ^ param_1[0xe] ^ string[0xe]) == (param_1[0xf] ^ string[0xf])) {
			printf("\nSo far so good...\n\n");
			int var2 = 0;
			int var1 = (param_1[0xb] ^ string[0xb]) * 0x88 + 0x13a;
			while (var2 != 0x10) {
				var1 -= param_1[var2+0x10];
				var2++;
			}

			if (var1 == 0) {
				printf("\nYipee\n\n");
				return true;
			}
		}
	} else {
		printf("len error");
	}
	return false;
}

char * keygen() {
	char * chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890";
    char * ret = malloc(0x41 * sizeof(char));
    int i = 0;
    int sel;
    char * rest = "YYYYYYYYAAAAAAAABBBBBBBBZZZZZZZZ";

    while (i < 0x40) {
		if (i >= 0x20) {
			i++;
        } else {
            sel = rand() % strlen(chars);
            ret[i] = chars[sel];
			i++;
        }
    }

	strcat(ret, rest);
    
    return ret;
}

int main() {
	char * key;

	while (true) {

		key = keygen();
		printf("%s\n", key);
		if (check(key) == true) {
			printf("This works!");
			return 0;
		}
	}
}

```

You'll have to wait a minute until it generates...

You should get a key like: ```44ABcOrOE7M2mO4gFNPgt9NY1J5ehD1aYYYYYYYYAAAAAAAABBBBBBBBZZZZZZZZ```
