In this crackme we have a README included. Our job is to find out what the utility program (util) and the vol1-5 files. We also need to find if there is a way to extract vol3 which is missing.

## util file

I started by opening radare2 and using the afl command for the util file

```bash
$ r2 util
[0x00001160]> aaaa
...   # output
[0x00001160]> afl
```

The output of this command shows that the following external C functions are used:
- puts
- fread
- fopen
- fclose
- printf
- exit
- fwrite
- strcmp

So, now we can open up the main function to see whats going on with the ```pdf@main``` command. Some of the first few lines are as follows:

```bash
... # unimportant
mov dword [var_4h], edi   # argc
mov qword [s1], rsi   # argv
cmp dword [var_4h], 2
je 0x185a
```

We can see that the contents of the register edi, which has the value of argc, is moved into a variable named var_4h. Using cmp var_4h is compared to 2 and if they are equal the jump occurs.
If the jump does not happen the following code is run:

```bash
lea rdi, str.Usage:__s__command__n   # "Usage %s [command]\n"
mov eax, 0
call sym.imp.printf
mov edi, 0
call sym.imp.exit
```

We can see the code prints out "Usage %s [command]\n" where it seems %s would be argv[0] (filename) and then executes the exit() function with the 0 argument. This is also helpful because we can see that the executable is used by running: ```./util [command]```.

But if we look at the code after the jump we can see the below code:

```bash
mov rax, qword [s1]
add rax, 8
mov rax, qword [rax]
lea rsi, str.extract   # "extract"
call sym.imp.strcmp
test eax, eax
jne 0x188c
```

We can see that when argv[1] is not equal (jne) to "extract" (determined using strcmp() function) the jump would occur. If it doesn't we see this code:

```bash
mov eax, 0
call sym.extract
mov edi, 0
call sym.imp.exit
```

We see here that the extract() function is called and then the exit function is called. We can see that the extract function is created inside of the file. We will look at this later. Otherwise the program continues. We can see the code below:

```bash
mov rax, qword [s1]
add rax, 8
mov rax, qword [rax]
lea rsi, str.check   # "check"
call sym.imp.strcmp
test eax, eax
jne 0x188c
```
We can see the same is happening here with the string "check". Predictably, the check function gets called when the command is "check".

Otherwise the below code is run:
```bash
mov rax, qword [s1]
add rax, 8
mov rax, qword [rax]
mov rsi, rax
lea rdi, str.unknown   # "Unknown Command %s\n"
mov eax, 0
call sym.imp.printf
mov eax, 0
leave
ret
```

As we can see, the printf() function is run and it prints: "Unknown Command %s" where %s would be argv[1].

Now we will investigate the extract() function and oh boy is it long. I will not go over all of it, so I will just explain generally what it does.

It first checks if all of the vol1-4 exist in addition, it creates an out.bin file for writing purposes. There is then a loop where it uses the fread() function to save the value read to the out.bin file. It seems vol5 is not meantioned at all in this file.

The check() function seems to just check if vol1-5 are written correctly by looping through every character in the file using fread().

So, it seems the utils file writes the contents of the vol1-4 files to a file called out.bin if argv[1] == extract, if argv[1] == check it checks if the files exists and makes sure they are written correctly. I was unable to use radare2 on the vol files because they don't seem to be in the form of any file Linux understands. They are surely not ELF files.
