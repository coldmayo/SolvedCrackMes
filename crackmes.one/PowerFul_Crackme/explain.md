We will take a look at main first:

```bash
movabs rax, 0x626f676a61667062   # "bpfajgob"
mov qword [var_17h], rax
mov word [var_fh], 0x7769   # "iw"
mov byte [var_dh], 0
lea rdi, str.The_magic_string:   # "The magic string: "
mov eax, 0
call sym.imp.printf
mov dword [var_4h], 0
```
It seems "bpfajgob" is saved into var_17h and "iw" is saved to var_fh. Then "The magic string: " is printed.

After this, user input is taken with scanf("%c") in a loop.

Then we see the following loop:

```bash
char user_input[28];
for (int i = 0; i < 10; i++) {
	if (i < 7) {
		user_input[i+3+7] = user_input[i+0x12];
	} else {
		user_input[i-7+7] = user_input[i+0x12];
	}
}
```

After this we have the checker:

```bash
user_input[17] = '\0';
int j = 0;
while (true) {
	if (9 < j) {
		printf("Congratulations, correct flag!\nThe flag is: WatadCTF{%s}\n", user_input+0x12);
		return 0;
	}
	if (user_input[j+7] != var_17h[j]) {
		break;
	}
	j++;
}
```

It seems that the last 3 characters are moved to the front of the string and the code checks if this string is equal to bpfajgobwi.

So, I think the key is ajgobiwbpf.
