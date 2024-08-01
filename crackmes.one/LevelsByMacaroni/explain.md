## Answer (TL;DR)

When promped for String #1 input: 34nis

When promped for String #2 input: 34nis

Patch the file by changing:
```bash
cmp dword [var_8h], 0x1b39 
```
to 
```bash
cmp dword [var_8h], 6
```
Once this is done, when you are prompted to "type a letter then press enter" you will be able to enter anything (even non letters) and get the success prompt.

## Explination

This isn't my first crackme, but this is my first time submitting a solution. Sorry if my explinations aren't great.

Using Radare2 we can see that the external functions used are:
- puts
- strlen
- printf
- fgets

Doesn't seem to tell us much, yet.

When we open up the main function, we can see the following assembly code:

```bash
lea rax, str.string   # Enter a string
mov rdi, rax
mov eax, 0
call sym.imp.printf
mov rdx, qword [obj.stdin]
lea rax, [s]
mov esi, 0x28
mov rdi, rax
call sym.imp.fgets
```

Here we can see that "Enter a string" is printed using the printf function and then uses the fgets function to retrieve input from the user.

Right after this we see the below code

```bash
lea rax, [s]
mov rdi, rax
call sym.imp.strlen
mov dword [var_8h], eax
cmp dword [var_8h], 6
jne 0x1416
```
The code seems to take the output from fgets, check the length of the string, and if it is not equal (jne) to 6 a jump occurs. If the jump does occur the puts function is used to print "FAILURE-LVL-1: how long is your string? Do you know what a NULL terminator is?". The program ends.

If the jump does not occur the puts function is used to print: "\n\n\nLevel 1 Pass: String length is correct\n\n". The program continues with the following code:

```bash
movzx eax, byte [var_3fh]
cmp al, 0x34   # '4'
jne 0x1405
```
What I get from this is that one of the characters in the inputted strings has to be equal '4'. I may be an assembly noob, but I wasn't sure by just glancing. It doesn't help that var_3fh isn't meantioned previously. I "cheated" and peaked at the Ghidra decompilation.
It showed that the 2nd character in the string had to be of the value '4'. If it is not, "FAILURE-LVL-2: Maybe the program wants a certain character in a certain position to proceed?\n\n" is printed. Otherwise the string passes to the next level and prints: "Level 2 Pass: the character is as expectedd\n\n"

```bash
movzx eax, byte [s]
cmp al, 0x33   # '3'
jne 0x1405
```
So, we now know we need a string that starts with 3 to continue to the next level. If your inputted string meets this and all previous requirements the following is printed out: "Level 2.5 Pass: You understand the requirement\n\n", "you will be prompted to enter a second key..... I know. Annnnoyyyingggg", and finally "The first key and the second key have a certain relationship figure this out to pass\n\n"

```bash
lea rax, str.enter   # Enter string #2
mov rdi, rax
mov eax, 0
call sym.imp.printf
mov rdx, qword [obj.stdin]
lea rax, [var_70h]
mov esi, 0x28
mov rdi, rax
call sym.imp.fgets
```

In this code we can see it print "Enter string #2" and then use fgets to retrieve the answer.

```bash
lea rax, [var_70h]
mov rdi, rax
call sym.imp.strlen
mov dword [var_ch], eax
mov eax, dword [var_ch]
cmp eax, dword [var_8h]
jne 0x13e3
```

It seems that this code checks if the inputted string is the same length as the previous one. If they are not equal this is printed: "The string length has to be the same". If they are equal the program continues.

In the next code it seems there is a for loop where it loops over both strings and compares the chracters to each other. It checks if the numeric equivilents of of the characters in string #2 are not equal to twice the numeric equivalents of string #1. I hope that made sense.

If you succeed you will be greeted with this message: "SUCCESS-LVL-3: You Did it! i hope you learned something. i pray for the day crackmes.one adds a section for hints that the author can add

If you havent done it before this will be a little bonus section for binary patching
For the bonus section you need to modify the binary to bypass an impossible if statement
When ready type a letter then press enter to go into the statement:"

After checking, Mr. Macaroni is correct, the only way to "win" is for the ```cmp``` to believe that the length of first string is equal to 0x1b39. We will need to patch the executable to fix this. I did this by patching it in Ghidra by switching this line:
```bash
cmp dword [var_8h], 0x1b39 
```
to 
```bash
cmp dword [var_8h], 6
```
and it should work no matter what letter is pressed when promped.

When this succeeds the following is printed: "SUCCESS-LVL-BONUS: Well damn you did it Should I make more of these? Probably not with the yanderedev if statement spam and 7 million calls to printf, but ill work on it Bye! - saltedMacaroni841"

We did it, yay


