It seems this crackme has one function other than main(), which is test(). I guess we'll keep that in mind.

I used Radare2 for disassembly purposes. argc is saved into var_14h and argv is saved into var_20h. We see the following:

```bash
cmp dword [var_14h], 2
jne 0x1220
```
This jumps to the end of the program and prints "Try more ... 1/5". So we need to put in one argument ("Shocker"). We will call this argument the "key" from now on. After taking a glance at Ghidra I found the below code:

```bash
char * name = * argv;
if ((int)*name + (int)*argv[1] == 0x6e) {
	...
}
```

It seems after this the code checks if the first letter of the program name + first letter of the key equal to 0x6e or 110 in decimal form.

argv[0] is in the form of ./level1 so the first character is '.' (ascii code 46) to equal 110 is '@' (ascii code 64).

but inside of the for loop we see:

```bash
if ((((name[3] == 'n') || (name[4] == 'i')) || (name[5] == 'm')) || name[6] == 'a') {
	test((int)name[3], (int)*param2[1]);
} else {
	printf("Try more .... 3/5");
}
```

It seems that we need to change the name of the file so that it either has character 'n' as the 2nd character, 'i' as the 3rd character, 'm' as the 4th character, or 'a' as the 5th character. If this is true, we can continue to running the test() function. It has name[3] as the 1st argument and the key as the 2nd argument.

In the test function we see:

```bash
if ((param_1 == ':') || (param_2 == '@')) {
	printf("true point 5/5");
} else {
	printf("false point 4/5");
}
return;
```
It seems that our solution is changing the name of the file to lnvel1 and the passcode is @.
