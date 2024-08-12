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
		one = rand() % 30;
		two = rand() % 30;
		printf("one: %d, two %d\n", one, two);
		if (math_check(one, two)) {
			printf("This one works");
			return;
		}
	}
}
