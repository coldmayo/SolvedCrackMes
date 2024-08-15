Well I noticed this was a big file. Great.

I chose to check out main first:

```bash
mov dword [var_34h], edi   # argc
mov qword [var_40h], rsi   # argv
mov rax, qword fs:[0x28]
mov qword [var_8h], rax
xor eax, eax
cmp dword [var_34h], 2
je 0x401d73
```

This code checks if argc == 2. If it doesn't, it prints: "Use ./WarGames pass". Otherwise:

```bash
mov rax, qword [var_40h]
add rax, 8
mov rax, qword [rax]
mov rdi, rax
call fcn.00401180   # strlen
cmp rax, 9
je 0x401da2
```

The length of the inputted password (argv[1]) needs to equal 9. Otherwise "Wrong Password !!!" is printed. Otherwise:

```bash
movabs rax, 0x6370742377737367
mov qword [var_11h], rax
mov byte [var_9h], 0x7a
mov dword [var_24h], 0
mov dword [var_28h], 0
mov edi, 0x7bf
call sym.srandom
mov qword [var_20h], 0
```

It seems that var_11h is set to 0x6370742377737367 (gssw#tpc), var_9h is set to 0x7a, var_24h is set to 0, and var_28 is set to 0. Then the srandom function is used. It is used to set a seed (0x7bf) for the rand() function used later.

```bash
int random;
for (int i = 0; i < 9; i++) {
	random = rand();
	var_11h[i] -= (random % 5) + '1';
	if (var_11h[i] != i + argv[1]) {
		printf("Wrong Password !!!");
		return 0;
	}
}
printf("Congratulation !!!");
```

I was able to quickly write a keygen:

```bash
#include <stdio.h>
#include <stdlib.h>

int main() {
	srand(0x7bf);
	char plebus[] = "gssw#tpc\0";
	int random;
	char * pass = malloc(10 * sizeof(char));
	for (int i = 0; i < 9; i++) {
		random = rand();
		plebus[i] -= (char)((random % 5) + '\x01');
		pass[i] = plebus[i];
	}
	printf("%s\n", pass);
	free(pass);
	return 0;
}

```

This prints "dont play" which is the password!
