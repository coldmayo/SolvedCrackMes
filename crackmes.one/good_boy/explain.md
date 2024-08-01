## Answer (TL;DR)

Password: h4x0r

## Explination

According to the posting, the goal is to find the password. First, I will reverse enginner the executable to fully understand the code. I will be using Radare2.

First, I took a look at the functions in the code:
- puts
- printf_chk
- scanf

and main of course. Now, I am going to analyze main.

The first few lines of the assembly are:

```bash
mov edx, 0x72   # 'r'
mov edi, 1
mov rax, qword fs:[0x28]
mov qword [var_118h], rax
xor eax, eax
lea rsi, str.Enter   # Enter password
mov word [var_eh], dx
mov dword [var_ah], 0x30783468   # 'h4x0'
call sym.imp.__printf_chk
```

So, it seems that 'r' is first moved into the edx register, the string "Enter password" is printed, and 'h4x0' is moved into var_ah. After this, we see that a scanf call for taking input. After that we see the following code:

```bash
mov eax, dword [var_10h]
cmp dword [var_ah], eax
je 0x1147
```

It seems that the user input is compared to var_ah (h4x0) and if they are the same, the jump occurs. If the jump does not occur, "Bad boy" is printed. The code jumped to is:

```bash
movzx eax, word [var_14h]
cmp word [var_eh], ax
jne 0x111e
lea rdi, str.good   # 'Good Boy'
call sym.imp.puts
```
It seems that var_14h is moved into eax while making the higher bits 0. Then the lower 16 bits of eax is compared to var_eh ('r'). If they are the same then we get the success prompt. What I take from this is the password is 'h4x0' + 'r' or 'h4x0r'. If we try this out, it seems to work! I am now a good boy.
