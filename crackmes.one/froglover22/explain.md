First I looked at the main function:

(var_8h is argc, var_10h for argv)
```bash
cmpdword [var_8h], 2
jne 0x11ba
mov rax, qword [var_10h]
mov rdi, qword [rax]
call sym.imp.atoi
mov dword [var_14h], eax
- 0x11ba
```

It seems that it checks if argv is equal to 2. If it is equal to two then the jump does not occur. If it does not atoi is called on argv[1] and saves it to var_14h.

After that there is a loop, the below Ghidra code represents this:
```bash
for (i = 0; i < 5; i = i + 1) {
	if ((var_14h & 1 << ((byte)i & 0x1f)) != 0) {
    	for (j = 0; j < 8; j = j + 1) {
    		*(byte *)(*(long *)(chunks + (long)i * 8) + (long)j) = *(byte *)(*(long *)(chunks + (long)i * 8) + (long)j) ^ 5;
    	}
	}
}
```

The outer loop loops through i = 0 to i = 4
- var_14h & 1 << ((byte)i & 0x1f)
	- 1 << ((byte)i & 0x1f) creates a bitmask where only the bit corresponding to the index i is set
	- var_14h & (bitmask) checks this bit is set in var_14h. If it is the result is != 0 if it is it == 0
	- Example:
		- i = 1:
			- 1 << ((byte)1 & 0x1f); 1 << (1); result: 0b00010 (or 2)
			- var_14h & (2); lets say var_14h is 11; 0b01011 & 0b00010 = 0b00010
			- 0b00010 is non-zero so the bit is set, inner loop triggered
The inner loop iterates through i=0 to i=7
- *(long *)(chunks + (long)i * 8)
	- we will say that chunks is an array
	- calculates the address of the i-th element in this array of addresses
	- *(long *) dereferences this address to get a pointer to a chunk of data
- *(byte *)(*(long *)(chunks + (long)i * 8) + (long)j)
	- Accesses byte in the ith chunk with offseting the address by j bytes to access the jth byte in the chunk
- ^ each byte is XORed with 5

So a better for loop would be:

```bash
for (i = 0; i < 5; i++) {
    if ((var_14h & (1 << i)) != 0) {
        for (j = 0; j < 8; j++) {
            chunks[i][j] = chunks[i][j] ^ 5;
        }
    }
}
```

Once the loop is done we see the following code:
```bash
lea rdi, [s]
xor esi, esi
mov edx, 0x27
call sym.imp.memset
```

s here is a character buffer of 52 in length. We modify it with a memset which sets the first 39 elemnts with '\0'

Next we find actual definitions of the chunks:
```bash
lea rdi, [s]   # char * s
mov esi, 0x27   # size_t n
lea rdx, str._s   # %s%s%s%s%s
lea rcx, obj.chunk_one   # "`fmj%'Fjkbwdqv$%Youve fopka%qm`%flag.\""
lea r8, obj.chunk_two   # "kbwdqv$%Youve fopka%qm`%flag.\""
lea r9, obj.chunk_three   # "Youve fopka%qm`%flag.\""
lea r10, obj.chunk_four   # "pka%qm`%flag.\""
lea rax, obj.chunk_five   # "flag.\""
mov qword [rsp], r10
mov qword [var_sp_8h], rax
mov al, 0
call sym.imp.snprintf
```

So it seems that using snprintf is used to connect the chunks together into the s buffer

```bash
lea rdi, [string]
xor esi, esi
call sym.imp.memset
lea rdi, [string]
lea rcx, [s]
mov esi, 0x50
lea rdx, str.sh   # sh -c '%s' 2>/dev/null
mov al, 0
call sym.imp.snprintf
```
This seems to set the first 80 characters to '\0' in the character buffer named string. Then uses snprintf to insert the s buffer where '%s' is.

```bash
lea rdi, [string]
call sym.imp.system
cmp eax, 0
je 0x12f3
```

Calls the system function to run the system() function to run the string named string in the command line. If it succeeds in running it makes the jump. If it doesn't it prints "Incorrect".

Otherwise it simply just returns 0.

So the input has to be a number. This determines the output, which if it classifies as an actual command, the command will print the good boy prompt.

Because of this, we need to find the arguements where the first 5 characters of the output are: "echo ". I did this with the following script:

```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * slice_str(char * str, char * buffer, int start, int end) {
    int j = 0;
    for (int i = start; i <= end && str[i] != '\0'; ++i) {
        buffer[j++] = str[i];
    }
    buffer[j] = '\0';
    return buffer;
}

int find_int(int mask) {
    char string[80];

    char chunks[5][80] = {
        "`fmj%'Fjkbwdqv$%Youve fopka%qm`%flag.\"",
        "kbwdqv$%Youve fopka%qm`%flag.\"",
        "Youve fopka%qm`%flag.\"",
        "pka%qm`%flag.\"",
        "flag.\"",
    };

    for (int i = 0; i < 5; i++) {
        if ((mask & (1 << i)) != 0) {
            for (int j = 0; j < 8 && chunks[i][j] != '\0'; j++) {
                chunks[i][j] = chunks[i][j] ^ 5;
            }
        }
    }

    snprintf(string, sizeof(string), "%s%s%s%s%s", chunks[0], chunks[1], chunks[2], chunks[3], chunks[4]);

    //printf("%s\n", string);

	char buffer[6];
	//printf("%s\n", slice_str(string, buffer, 0, 4));
	if (strcmp(slice_str(string, buffer, 0, 4), "echo ") == 0) {
		
		return 1;
	}

    return 0;
}

int main() {
	int res;
	for (int i = 0; i < 200; i++) {
		res = find_int(i);
		if (res == 1) {
			printf("%d\n", i);
		}
	}
	printf("done");
	return 0;
}

```

If you run this it seems every odd number starts with echo. After some expirmenting, I found that "Congrats, Youve found the flag" is what the correct answer would give. After running the executable, I found that 11, 43, 75, 107, 139, 171, and 195 returned the string. I believe these work because the first five bits are 01011.
