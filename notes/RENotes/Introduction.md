Go to [[Prereqs]] first to gain the knowledge and software to start Reverse Engineering.

In this tutorial, I will go over the process of how I personally go about solving crackmes. If you want an example of my thought process on a real example check out some of the crackmes with an explain.md

The first thing I would do is take a look at the functions in the code to get an idea of what's up using Radare2.

```bash
# start Ghidra
$ r2 ./exe_name

# analyze executable file
[0x000000]> aaaa

# veiw functions
[0x000000]> afl
```

This should show all of the functions used in the program. Functions that start with "sym.imp." are functions imported from the C standard library, the ones that start with "sym.fcn" are to be ignored, there should be a main() function and possibly some others.

If there is a main() function, I would more likely than not observe that one first.

```bash
[0x000000]> pdf@main
```

Now we can see the assembly code for this function. For a smaller script I would analyze it straight from Radare2. Otherwise I would open up Ghidra as well and analyize them side by side to get the full picture. Ghidra is a decompiler, so it will convert the executable to c code. This is helpful since it makes things more even more readable.

I like to take notes on what chunks of the code do because typing/writing helps me learn. After the notes are done I like to write a c file that can recreate the origional exe file.
