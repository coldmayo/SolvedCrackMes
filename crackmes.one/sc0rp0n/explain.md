## Answer (TL;DR)

Some keys that work:
- K10C-DPG2-N87X-XAKO
- 4N10-5XE5-6SBJ-RU5W
- JGI5-S3JE-SDPL-YWTW
- A51H-V31S-W5ZQ-KWLK

and more...

## Explination

Using radare, I checked out the main() function. The first thing I noticed:
```bash
cmp dword [var_4h], 2   # var_4h is argc
je 0x1333
lea rax, str.usage   # "Usage <key>"
mov rdi, rax
call sym.imp.puts
mov eax, 0
jmp 0x139f
```
If argc does not equal 2, then "Usage <key>" is printed using puts() and then jumps to the end of the program. So, we need 1 command line arguement. Next we see:
```bash
mov rax, qword [var_10h]   # var_10h is argv
add rax, 8
mov rax, qword [rax]
mov rdi, rax
call fcn.0000126e
xor eax, 1
test al, al
je 0x1363
lea rax, str.inv   # "Invalid key!""
mov rdi, rax
call sy.imp.puts
mov eax, 0
jmp 0x139f
```
It seems that argv[1] is inputted into function fcn.0000126e. If the output of it is equal to 1, then the je succeeds. If it does not, then it prints "Invalid key" and jumps to the end of the code. Next we see:
```bash
mov rax, qword [var_10h]
add rax, 8
mov rax, qword [rax]
mov rdi, rax
call fcn.000011c0
test al, al
je 0x138b
lea rax, str.yippee   # "Access granted!"
mov rdi, rax
call sym.imp.puts
jmp 0x139a
lea rax, str.boo   # "Invalid key!"
mov rdi, rax
call sym.imp.puts
```
It seems that argv[1] is passed into fcn.000011c0 and if it does not return 0 we get the good boy prompt: "Access granted!" if it returns 0 we see: "Invalid key!" and then the program ends.

At this point we should probably take a look at fcn.000011c0 and fcn.0000126e.

First, fcn.0000126e:

It seems at first sVar1 is set to strlen() of the 1st (and only arguement). We can then see the following code:
```bash
mov ecx, dword [0x00004010]
mov edx, dword [0x00004014]
imul ecx, edx
mov edx, dword [0x00004010]
add edx, ecx
sub edx, 1
movsxd rdx, edx
cmp rax, rdx
je 0x12b3
mov eax, 0
jmp 0x1302
```
I checked Radare2 and Ghidra and they both seem to agree that the data stored at both 0x00004010 and 0x00004014 is 4 (integer). It seems that it checks if 4+4*4 - 1 is equal to the strlen() value. If it is the je succeeds, if it doesn't the function returns 0 (we don't want this). So we need a key with a strlen of 19. Next we see a loop of which I tranlated to C code:
```bash
for (int i = 0; i < 4 - 1; i++) {
	if (param_1[4 + (4 + 1) * i] != '-') {
		return 0;
	}
}
ret = 1;
```
This tells us we need dashes in our key at index 4, 9, and 14. So the key has to take the form of XXXX-XXXX-XXXX-XXXX. Otherwise it returns 0. If it is in the correct format it will return 1 (which is what we want).

Now to analyze fcn.000011c0:

I have represented what the function is doing in the C code below:
```bash
int j = 0;
int i = 0;
int k;
while (true) {
	if (4 <= i) {
		return 1;
	}
	k = 0;
	for (int l = 0; l < 4; l++) {
		k = k + param_1[l+i * 5];
	}
	if (fcn.00001169(k) != 1) {
		break;
	}
	if (k <= j) {
		return 0;
	}
	j = k;
	i++;
}
return 0;
```
Lets break this down:
- When i reaches 4 it returns 1 (what we want)
- Adds characters from param_1 to k
- fcn.00001169 needs to return 1
- if k is less than or equal to j then it returns 0
- j = k at the end of the loop
- i is incremented at the end of the loop

Now we must look at fcn.00001169:
```bash
cmp dword [var_14h], 0   # var_14h is param_1
je 0x1180
cmp dword [var_14h], 1
jne 0x1187
mov eax, 0
jmp 0x11be
```

It seems param_1 cannot be 0 or 1, otherwise it will return 0 (we do not want this)

Otherwise, a loop as seen below is run:
```bash
for (int i = 2; i < param_1/2; i++) {
	if (param_1 % i == 0) {
		return 0;
	}
}
res = 1;
```
Seems to check the divisablility from i = 2 to i = param_1/2 and if it is not divisable then return 1. So, we need to make sure param_1 is prime.

Ok now we have know everything we possibly can about this program. It's time to make a keygen.

After this, I constructed a keygen:
```bash
import random

def is_prime(num):
    if (num == 0 or num == 1):
        return False
    for i in range(2, num//2 + 1):
        if (num % i == 0):
            return False
    return True

def val_key(key):
    i = 0
    j = 0
    while True:
        if (4 <= i):
            return True
        summ = 0

        for k in range(4):
            summ = summ + ord(key[k+i*5])
        if not is_prime(summ):
            return False
        if (summ <= j):
            return False
        j = summ
        i += 1

def main():

    while True:
        part1 = "".join(random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for _ in range(4))
        part2 = "".join(random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for _ in range(4))
        part3 = "".join(random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for _ in range(4))
        part4 = "".join(random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789') for _ in range(4))

        key = part1+'-'+part2+'-'+part3+'-'+part4

        print(key)

        if (val_key(key)):
            print("This key works!")
            return 0

if __name__ == "__main__":
    main()
```

Once this is run you should have a key that works. For example, I got K10C-DPG2-N87X-XAKO which worked!

Thanks for the crackme. Very fun stuff.
