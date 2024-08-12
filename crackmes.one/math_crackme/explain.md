The first thing I did was check out the functions in the executable:
- entry0
- fcn.004010cf
- fcn.004010e9
- fcn.0040111d

So, I guess we'll check out entry0 first:
```bash
movabs rsi, str.enter_first_number:_enter_second_number:_CORRECT_nWRONG_n   # "enter first number: ", "enter second number", "CORRECT\n", "WRONG\n"
movabs rbx, 0x14
call fcn.004010cf
call 0x4010b2
call fcn.004010e9
```
From what I have seen from both Ghidra and Radare2, fcn.004010cf does nothing but do a syscall() and then return 1. But, fcn.004010e9 is more interesting:
```bash
char * value = &DAT;
while (*value != '\n') {
	*value -= 0x30;
	value += 1;
}
return;
```
I am not %100 sure what DAT is, but I assume what the chunk of code does is use fcn.004010cf to print "enter first number" and then fcn.004010e9 is used to take user input. Also that the original code is in assembly. Then the same thing is done for "enter second number". Next, we see:

```bash
mov qword [0x402008], rdi   # 0x402008 is the second number
call fcn.0040111d
cmp rdi, qword [0x402008]
je 0x401083
movabs rbx, 6
movabs rsi, 0x402055
call fcn.004010cf
jmp 0x40109c
movabs rbx, 8
movabs rsi, 0x40204d
call fcn.004010cf
```

What this code does is call fcn.0040111d and then compare its output to the second number. If it is zero, then we get the message "CORRECT", otherwise, we get "WRONG".

So we need to check out fcn.0040111d:
```bash
mov rax, qword [data]
movabs rbx, 2
xor rdx, rdx
div rbx
mov rdi, rdx
xor rdx, rdx
```

Ok so, I believe the first number is taken in and saved to rax, and then it sets the divisor to 2, rdx is set to 0, the first number is divided by 2, the quotent is moved into rdi, and rdx is set to 0.

```bash
mov rax, qword 0x402005
movabs rbx, 3
div rbx
mov rbx, rdx
movabs rsi, 6
and rdx, 1
mov cl, dl
shl rbx, cl
inc rbx
mov rax, rsi
xor rdx, rdx
div rbx
xor rdi, 1
mul rdi
mov rdi, rax
ret
```

This one is longer, so here is the C code:

```bash
uint64_t rax, rbx, rdx, rsi, rdi;
rax = one;   # first number
rbx = 3;
rdx = rax % rbx;
rax = rax / rbx;
rbx = rdx;
rsi = 6;
rdx = rdx & 1;
uint8_t cl = rdx;
rbx = rbx << cl;
rbx += 1;
rax = rsi;
rdx = 0;
rdx = rax % rbx;
rax = rax / rbx;
rdi = 0;
rdi ^= 1;
rax = rax * rdi;
rdi = rax;
return rdi;
```
Ok cool, so we know the math now. Now we can make a keygen:
```bash
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

bool math_check(uint64_t one, uint64_t two) {
    uint64_t rax, rbx, rdx, rsi, rdi;
    rax = one;
    rbx = 2;
    rdx = 0;
    rdx = rax % rbx;
    rax = rax / rbx;
    rdi = rdx;
    rdx = 0;
    rax = one;
    rbx = 3;
    rdx = rax % rbx;
    rax = rax / rbx;
    rbx = rdx;
    rsi = 6;
    rdx = rdx & 1;
    rbx = rbx << rdx;
    rbx += 1;
    rax = rsi;
    rdx = 0;
    rdx = rax % rbx;
    rax = rax / rbx;
    rdi ^= 1;
    rax = rax * rdi;
    rdi = rax;
    printf("%ld\n", (long)rdi);
    if ((long)rdi == (long)two) {
        return true;
    }
    return false;
}

void main() {
	while (true) {
		int one, two;
		one = rand() % 5;
		two = rand() % 5;
		printf("one: %d, two %d\n", one, two);
		if (math_check(one, two)) {
			printf("This one works");
			return;
		}
	}
}
```

If we run this we find that the first number should be 3 and the second should be 0. If we test it, it indeed works.
