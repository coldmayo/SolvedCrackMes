So, I guess we'll start by taking a look at main:

```bash
lea rax, str.string   # "Welcome, please enter the password to continue."
mov rdi, rax
call sym.imp.puts
mov rdx, qword [obj.stdin]
lea rax, [s]
mov esi, 0x400
mov rdi, rax
call sym.imp.fgets
```

It seems that the code prints "Welcome, please enter the password to continue" and then uses fgets() to take in user input.

Then the following for loop occurs:

```bash
char string[32];
int len = strlen(user_input);
for (int i = 0; 0x20 > i; i++) {
	string[i] = (char)len;
}
```
Then:

```bash
mov eax, dword [len]
and eax, 0xfffffff0
cmp eax, 0x20
je 0x1de9
```

So, this means that the length of the input needs to be able to have the and operated on it and 0xfffffff0 and equal to 0x20. After doing some calculation, the length needs to be between 32 and 47. If it is not, the program prints "Sorry, I have to deny you access.".
Otherwise, the fcn.00001cba is called where fcn.00001cba(user_input, len & 0xfffffff0, string, 0x20)

We'll get back to that later:

```bash
char * dat = "\xef\0\0\0all those wh\x6f\0\0\0nder are los";
for (int i = 0; i < 0x20; i++) {
	if (user_input[i] != dat[i]) {
    	puts("Sorry, I have to deny you access");
    	return 1;
	}
}
```

It seems that if the user_input is not equal to "\xef\0\0\0all those wh\x6f\0\0\0nder are los" then the program prints "Sorry, I have to deny you access.".

Otherwise, it prints "Welcome, you have been granted access."

Now we must check out fcn.00001cba.

## fcn.00001cba

Seems to be as follows:

```bash
int fcn_00001cba(char * user_input, int param_2, char * string, int num) {
	int i = 0;
	int ret;
	if ((num == 0x20) || ((num < 0x21 && ((num == 8 || (num == 0x10)))))) {
		for (int j = param_2; 0 < j; j = j - 0x10) {
			fcn_00001169(i + user_input, string, num);
			i = i + 0x10;
		}
		ret = 0;
	} else {
		ret = 1;
	}
	return ret;
}
```

It seems that if the length of user_input is equal to 0x20 (32), 8, or 0x10 (16) then the code loops through every 16th character and plugs it into fcn_00001169. But as we know, the length has to be 32.

You'll notice that in dat 0xEF is at index 0 and 0x6f is at index 16.
## fcn_00001169

```bash
void fcn_00001169(char * user, char * string, int num) {
	int var1 = user_c + 4;
	int var2 = user_c + 8;
	int var3 = user_c + 0xc;
	if (num == 0x20) {
		int var4 = string + 4;
		int var5 = string + 16;
		int var6 = string + 24;
		*string = *string >> 0x38 | (ulong)((uint)(*string >> 0x28) & 0xff00) | (ulong)((uint)(*string >> 0x18) & 0xff0000) | (ulong)((uint)(*string >> 8) & 0xff000000) | (*string & 0xff000000) << 8 | (*string & 0xff0000) << 0x18 | (*string & 0xff00) << 0x28 | *string << 0x38;
		...
	}
	
	# other if statements don't matter, num will always be 0x20 (32)
}
```

What the function does:
- Byte Swap string
- input_buffer is XORed with the reversed string
- 24 bit rotation for user_c
- he last step involves a transformation where each block is rotated right by 12 bits and a bitwise XOR is applied to the least significant byte.

So we know some of the confirmed characters in the password:
____all those wh____nder are los

Now to figure out what the password is, we need a keygen. You can find that in keygen.c

The coolest answer is: Not all who wander are lost;
