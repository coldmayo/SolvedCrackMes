## Answer (TL;DR)

Enter these as command line arguements: ABCDEFGHIJKL 24

## Explain

The first thing I did was check out the functions in the program. These are the external functions:
- puts
- strlen
- strcspn
- atoi

Internal:
- main
- check_key
- encode_input

We will check out the main function first:

```bash
mov dword [var_14h], edi   # this is argc
mov qword [str], rsi   # this is argv
cmp dword [var_14h], 2
jg 0x13ea
```

It seems that if argv == 2 then the jump occurs. If the jump does not happen the following is printed: "usage: ./rust-1 key number" and main returns 0.

If the jump occurs:

```bash
mov rax, qword [str]
mov rax, qword [rax + 8]
mov qword [s1], rax
mov rax, qword [str]
add rax, 0x10
mov rax, qword [rax]
mov rdi, rax
mov eax, 0
call sym.imp.atoi
mov dword [var_ch], eax
```
It seems this patch of code takes the 2nd element of argv and adds 0x10 to it and then turns it into an integer using the atoi function. This is then saved into the var_ch variable.

```bash
mov rax, qword [s1]
mov rdi, rax
call sym.imp.strlen
cmp rax, 0xc   # 12
je 0x1491
```
This code finds the length of the first argument and if that is equal to 12 then the jump occurs. If the jump does not happen "Access denied" is printed and main returns 0. If the jump happens the following code is run:

```bash
cmp dword [var_ch], 0
js 0x1445
cmp dword [var_ch], 0x32
jle 0x145b
```
This code snipet seems to make a jump if the second argument is not less than 0 and is greater than 0x32. If the jump does not occur "Access denied" is printed and main returns 0. If the jump occurs:

```bash
mov rax, qword [s1]
lea rdx, [0x00002047]   # \n
mov rsi, rdx
mov rdi, rax
call sym.imp.strcspn
mov rdx, qword [s1]
add rax, rdx
```
Seems like the sym.imp.strcspn is used to replaced all of the instances of \n with \0. After this, the check_key function is called with arguements: the key and number entered by the user.

In the check_key function we see the code:
```bash
mov qword [var_68h], rdi   # arg1
mov dword [var_6ch], esi   # arg2
mov rax, qword fs:[0x28]
mov qword [canary], rax
xor eax, eax
mov dword [var_54h], 0x7c
lea rdx, [s]   # arg3 (encode_input)
mov ecx, dword [var_6ch]
mov rax, qword [var_68h]
mov esi, ecx   # arg2 (encode_input)
mov rdi, rax   # arg1
call sym.encode_input
```
This snipet saves arg1 and 2 into var_68h and var_6ch respectfully, saved '|' into var_54h, create a string buffer which will be passed into the encode_input function along with the key and number.

The encode_input function seems to loop through the characters in the key, changes the character, and then saves them to the buffer.

I translated the encode_input function into C code below:

```bash
void encode_input(char * key, int num, char * buffer) {
	int i;
	for (i = 0; key[i] != '\0'; i++) {
		if ((i & 1) == 0) {   # and operation
			if ((key[i] & 1) == 0) {
				buffer[i] = key[i] - num;
			} else {
				buffer[i] = key[i] + num;
			}
		} else if ((key[i] & 1) == 0) {
			buffer[i] = key[i] + num*'\x02';
		} else {
			buffer[i] = key[i] + num*(-2);
		}
	}
	buffer[i] = 0;
}
```
Once the buffer is made, a while loop is formed which stops once the index i is greater than or equal to the length of the buffer. After this, it seems to check if ```buffer[i] == var_54h``` var_54h has 0x7c or '|' assigned to it.

We now know everything about this program, we can now generate a keygen with what we know:
```bash
def encode_input(key, num):
    buffer = []
    for i in range(len(key)):
        if (i & 1) == 0:
            if (ord(key[i]) & 1) == 0:
                buffer.append(chr(ord(key[i]) - num))
            else:
                buffer.append(chr(ord(key[i]) + num))
        else:
            if (ord(key[i]) & 1) == 0:
                buffer.append(chr(ord(key[i]) + num * 2))
            else:
                buffer.append(chr(ord(key[i]) + num * (-2)))
    buffer.append('\0')
    return ''.join(buffer)

for num in range(1, 35):
    test_string = "ABCDEFGHIJKL"
    encoded = encode_input(test_string, num)
    if encoded[-2] == '|':
        print(f"String: {test_string}, Number: {num}, Encoded: {encoded}")
        break

```

Running this will give us a solution: ABCDEFGHIJKL and 24

We did it, yippee!
