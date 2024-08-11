I will be using Radare2 (maybe Ghidra also) to analyize this executable. We can first take a look at the main() function:

```bash
lea rax, [0x00002020]   # ">> "
mov rdi, rax
mov eax, 0
call sym.imp.printf
lea rax, [var_40h]
mov rsi, rax
lea rax, str.string   # "%[^\n]%*c"
mov rdi, rax
mov eax, 0
call sym.imp.scanf
lea rax, [var_40h]
mov rdi, rax
sym.v1
```
It seems that the code prints ">> " and then takes in the user input. You'll notice the argument in the scanf: "%[^\n]%*c". The deal with this is it takes in any kind of character until the enter key is pressed. After that the user input is fed into a function called "v1". We'll come back to this.

```bash
test al, al
je 0x15b9
lea rax, str.yippee_one   # "WELL DONE 1\n>> "
mov rdi, rax
mov eax, 0
call sym.imp.printf
lea rax, [var_40h]
mov rsi, rax
lea rax, str.string   # "%[^\n]%*c"
mov rdi, rax
mov eax, 0
call sym.imp.scanf
lea rax, [var_40h]
mov rdi, rax
call sym.v2
```
If the output of the v1() function outputs "true" then the code continues, otherwise the executable stops running. If it does continue, it prints "WELL DONE 1\n>> " and takes more user input. It then feeds it into v2().

The same happens again, but the input feeds into v3().

The same seems to happen again, execpt the argument in scanf is "%c", which means it only takes a character as input and feeds into v4().

```bash
lea rax, str.nimacpp   # "nimacpp"
mov rsi, rax
lea rax, obj.last
mov rdi, rax
call sym.imp.strcmp
test eax, eax
jne 0x15b9
lea rax, str.great___you_won   # "great ! you won "
mov rdi, rax
call sym.imp.puts
```

But to get the "good boy" message we need to make sure last is equal to "nimacpp". No idea what last is but maybe we can find out from the other functions.

### v1
Now we are taking a look at the v1() function. Below I will translate the code to C code:
```bash
mov qword [s1], rdi
mov rax, qword [s1]
mov byte [rax], 0x6e   # 'n'
mov byte [last], 0x74   # 't'
mov dword [var_14h], 0
jmp 0x125e
```
So we now have a glimse of what last is. Seems that it saves 't' to last[1]. But also param1[0] has 'n' saved to it now. Then a for loop of the form of:
```bash
int len = strlen(param_1);
for (int i = 0; i < len; i++) {
	param_1[i] += (char)i;
}
```
Then:
```bash
mov rax, qword [s1]
lea rdx, str.n7t346
mov rsi, rdx
mov rdi, rax
call sym.imp.strcmp
test eax, eax
jne 0x12a3
mov rax, qword [s1]
movzx eax, byte [rax]
mov byte [obj.last], al
mov eax, 1
jmp 0x12a8
```

So now the program compares param_1 with "n7t346" and if it is then last[0] is set to "n". If param_1 == "n7t34b" then 1 is returned. Otherwise 0 is returned.

After checking an ascii table, it seems the password to this is n6r001, which is correct. Although, the first letter of the key could be anything, since it is set to 'n' anyway.

### v2
It seems that v2 takes a similar form to v1:
```bash
int len = strlen(param_1);
for (int i = 0; i < len; i++) {
	param_1[i] = param_1[i] + 4;
}
param_1[3] = 'w';   # Issue...
puts(param_1);
int ret = strcmp(param_1, "q77ivp5r");
if (ret == 0) {
	last[1] = param_1[3];
}
return ret == 0;
```

So we have a problem here, param_1[3] will be set to 'w' no matter what, which will mess us up. So, we will likely have to use breakpoints to skip this instruction. Looking at the disassembly:

```bash
0x57dde542430b      add rax, 3
0x57dde542430f      mov byte [rax], 0x77
0x57dde5424312      mov rax, qword [var_28h]
```
So, we will add a breakpoint at 0x57dde542430b and then resume at 0x57dde5424312

After consulting an ascii code table, it seems the correct password to this part is m33erl1n.

### v3
The code seems to take the form of:
```bash
char * m7 = "7m*$";
int len = strlen(m7);
for (int i = 0; i < len; i++) {
	param_1[i] = m7[i] + 2;
}
puts(param_1);
last[5] = 100;
int ret = strcmp(param_1, "9o,&Ji");
if (ret == 0) {
	last[2] = 'd';
}
return ret == 0;
```

So, it seems the password here would be 1234Ji. The first 4 characters don't matter anyway.

### v4
```bash
param_1 = param_1 - 3;
putchr(param_1);
if (param_1 == 'a') {
	last[3] = 'a';
}
return param_1 == 'a';
```

It seems the password here is simply: 'd'

But now we have a problem in main(), last does not equal "nimacpp". Seems like we'll have to patch the "JNE" to "JE".

After all of this we should get the message: great ! you won

We did it gamers.
