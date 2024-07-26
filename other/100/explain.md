Tools used:
- objdump
- radare2
- ghidra (optional)

The goal is to find out what the 100 executable does.

## The hard way

The first thing I did was use objdump to examine the file:
```bash
$ objdump -d 100
```

In it I was able to find these functions:
- puts
	- Used to write a string or line to the output stream (stdout)
- __printf_chk
	- It works simlilarly to printf but it checks for stack overflow before computing a result, depending on the value of the flag parameter.
- exit
	- allows for termination of a process with an input of the status (represents exit code)
- fopen
	- used to open a file to perform operations such as reading, writing, etc.
- fgetc
	- used to obtain input from a file, a single character at a time
- fclose
	- used to close an open file stream
- fwrite
	- writes data from the array pointed to, by ptr to the given stream
- perror
	- prints an error message to the standard error stream (stderr)

We could observe the assembly code from the objdump output, but radare might give a more information. We will first analyize the main function.

I followed the commands below:
```bash
# open Radare2

$ r2 100

# analyze using all functions using all normal and all experimental techniques

[0x000012c0]> aaaa

# open main function

[0x000012c0]> pdf@main
```

In the main function we can see that the first few lines are quite important:
```bash
...   # unimportant code above
push rbp
mov rbp, rsi   # put argv in the rbp register
push rbx
mov ebx, edi   # put argc in the ebx register
lea rdi, str.name   # Reverse Engineering Challenge #100 by dennis(a)yurichev.com
call sym.imp.puts
```

We can see that the puts function is called and prints out "Reverse Engineering Challenge #100 by dennis(a)yurichev.com".

Right below we find:

```bash
cmp ebx, 3
je 0x11a0
```

The cmp instruction subtracts the arguements and then the result can be used for conditional jump instructions (jz/je, jnz/jne, etc). We can see that the je conditional jump instruction is used. This means, if argc is equal to 3 one thing will happen, otherwise another will happen.

If we look back at the assembly code, we can see that if argc is not equal to 3 then the program executes the exit() function, ending the process. Otherwise, the program continues.

After that we can see that fopen is used twice, one with the "r" argument to open argv[1] and another with the "w" argument to open argv[1].

We can also see the assembly code below:

```bash
call sym.imp.fopen
mov r12, rax
call sym.imp.fopen
mov r13, rax
test r12, r12
je 0x1291
test rax, rax
je 0x12a4
```
As we can see, rax is used to store the return value of the fopen functions. The first one is saved to the r12 register, using the test instruction, if r12 has a value of 0 then the jump occurs. The same happens for rax (value equal to r13)

So basically, if fopen works for both files, then the program continues, otherwise the perror function triggers and returns "Error opening output file" or "Error opening input file".

In Radare2 we can see that there is at least 1 while loop. We can tell because of the below assembly code:
```bash
call sym.imp.fgetc
mov ebx, eax
cmp eax, 0x3c   # 0x3c = '<'
je 0x1277
cmp eax, 0x3e   # 0x3e = '>'
je 0x1259
cmp eax, 0x26   # 0x26 = '&'
je 0x123e
```

We can see that these jump to the fwrite function that writes &gt, &lt, and &amp respectfully.

Right below it we can see the following code:
```bash
mov edi, eax   # eax is where the return value of the fgetc function
call sym.imp.fputc
add ebx, 1
jne 0x11e0
```

What this code means is that if the value of the fgetc function is not equal to $-1$ then the loop will continue. But also that fputc will be used to write the character stored in the eax register. If the loop ends the program prints "%s written" where %s would be argv[2].

So now we have all of the possible information. The executable copies one file into another. It has the feature of converting >, <, and & to &lt, &gt, and &amp respectfully.

## The easy way
The alternative way you could've done this is by dragging and dropping the executable into Ghidra and decoding the C code. The only issue is you would have no way of knowing (to my knowlegde, anyway) that fwrite was writing the strings "&lt", "&gt", and "&amp". So, in this case I would recommend using Radare2 and then using Ghidra to check your work.
