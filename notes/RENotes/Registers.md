These notes will be for x86.

The register names are as follows:

| 8-byte register | Bytes 0-3 | Bytes 0-1 | Byte 0 |
| --------------- | --------- | --------- | ------ |
| rax             | eax       | ax        | al     |
| rcx             | ecx       | cx        | cl     |
| rdx             | edx       | dx        | dl     |
| rbx             | ebx       | bx        | bl     |
| rsi             | esi       | si        | sil    |
| rdi             | edi       | di        | dil    |
| rsp             | esp       | sp        | spl    |
| rbp             | ebp       | bp        | bpl    |
| r8              | r8d       | r8w       | r8b    |
| r9              | r9d       | r9w       | r9b    |
| r10             | r10d      | r10w      | r10b   |
| r11             | r11d      | r11w      | r11b   |
| r12             | r12d      | r12w      | r12b   |
| r13             | r13d      | r13w      | r13b   |
| r14             | r14d      | r14w      | r14b   |
| r15             | r15d      | r15w      | r15b   |

As you can see, there are 16 64-bit registers in x86-64.

Caller-save registers: rax, rcx, rdx, rdi, rsi, rsp, and r8-r11
- A caller-save register is used to hold temporary quantities which are not saved accross function calls
	- rax is used to store a function's return value
Callee-save ragisters: rbx, rbp, and r12-r15
- A callee-save register: Hold long lived values that should be preserved across calls.
	- rsp is the stack pointer (a pointer to the top element in the stack)
Parameter registers: rdi, rsi, rdx, rcx, r8, and r9
- used to pass the first six integer or pointer parameters to called functions
	- additional parameters are passed on the stack

