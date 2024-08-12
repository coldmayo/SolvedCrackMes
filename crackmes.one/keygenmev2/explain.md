First, I took a look at the functions in the program:
- strncpy
- abs
- strlen
- strchr
- printf
- read
- setvbuf
- __ctype_b_loc
- __cxa_finalize

Seems there are only external functions used (spoiler alert, I am lying). At first in main we see this:

```bash
mov dword [var_4h], 0
mov rax, qword [reloc.stdout]
mov rdi, qword [rax]
xor eax, eax
mov ecx, eax
mov edx, 2
mov rsi, rcx
call sym.imp.setvbuf
```
It seems that the setvbuf function is used on setvbuf where: ```setvbuf(stdout, NULL, _IOLBF, 0)```. This will be used for user input, I assume. Then we see the same for stdin. After that we see printf called a bunch of times, this shows:

```bash
*** Welcome to DZONERZY\'s keygenme v2! ***\n\n
| Rules:                           |\n
|                                  |\n
| - Patching is not allowed!       |\n
| - Keygen gen time < 10 min       |\n
| - Comment with proof valid key   |\n
|   for username \'DZONERZY\' and    |\n
|   result of commant \'time\'       |\n
| - You have to write a keygen!    |\n
------------------------------------\n\n
Enter your username: 
```
It was nice of Mr. DZONERZY to welcome us, we also know we have to write a keygen. Next we see:
```bash
lea rdi, [var_30h]   # arg1
mov esi, 0x20   # arg2
call fcn.00001a10
```
As you can see, there are internal libraries being used. I usually ignore the functions starting with "fcn". But, we can take a look and see what it's doing:
```bash
mov qword [var_8h], rdi   # arg1
mov qword [var_ch], esi   # arg2
mov dword [var_10], 0
mov eax, dword [var_10h]
cmp eax, var_ch
jge 0x1a7f
```
It seems that it takes the arguments and saves them into var_8h and var_ch. var_10h is set to 0. Then checks if eax is less than var_ch (parameter 2) and if it is the jump does not succeed.
```bash
xor edi, edi   # int fildes
lea rsi, [buf]   # void* buf
lea edx, 1   # size_t nbyte
call sym.imp.read
cmp rax, 0
jg 0x1a51
jmp 0x1a7f
```
Seems like the read() function is used to read a single character in stdin, if there is a character read the first jump occurs. Otherwise the second does.
```bash
movsx eax, [buf]
cmp eax, 0xa   # '\n'
jne 0x1a63
jmp 0x1a7f
```
The code seems to compare the character read to '\n'. If it is '\n' then the first jump does not happen. Otherwise the second jump happens.
```bash
mov dl, byte [buf]
mov rax, qword [var_8h]
movsxd rcx, dword [var_10h]
mov byte [rax+rcx], dl
mov eax, dword [var_10h]
add eax, 1
mov dword [var_10h], eax
jmp 0x1a26
```
It seems that var_10h gets added by 1, rax and rcx are set to var_8h and var_10h respectfully. buf was saved to dl. the character at the index rcx in rax is set to dl. After this it loops back to ```mov eax, dword [var_10h]```.
When the loop breaks (the second jmp's succeed):
```bash
mov rax, qword [var_8h]
movsxd rcx, dword [var_10h]
mov byte [rax + rcx], 0
mov eax, dword [var_10h]
add rsp, 0x20
pop rbp
ret
```
It seems that the only thing that is returned is var_10. In other words, it returns the number of characters read.

Back to main, in the next lines of code:
```bash
cmp eax, 0
jg 0x1bbe
```
If the length of the input is longer than 0 then the jump happens. Otherwise "Invalid Username" is printed and the program ends. If the jump occurs this code is run:

```bash
lea rdi, str.Enter   # "Enter your seriel:\n"
mov al, 0
call sym.imp.printf
lea rdi, [var_50h]
mov esi, 0x1e
call fcn.00001a10
```
This code gets the user's seriel numbers and inputs that and 0x1e into fcn.00001a10.

```bash
cmp eax, 0
jg 0x1c3b
```
If the length of the serial is 0 jump to end the program. Otherwise:
```bash
lea rdi, [var_30h]
lea rsi, [var_50h]
call fcn.00001980
```
This inputs the key and serial number into fcn.00001980. We will analyze that function now:
```bash
mov qword [var_10h], rdi   # arg1
mov qword [var_18h], rsi   # arg2
mov dword [var_1h], 0x1f
mov qword [var_28h], 0x3b9aca07
mov rdi, qword [var_18h]
call fcn.000014b0
```
It seems that arg2 is put into fcn.000014b0. It is quite long so I will just show it's C code below:
```bash
int FUN_001014b0(char *param_1) {
    int iVar1;
    size_t sVar2;
    char *pcVar3;
    int local_5c;
    int local_58;
    int local_54;
    int local_50;
    int local_44;
    int local_40;
    int local_3c;
    char local_36[5];
    char local_30[5];
    char local_2a;
    char local_29;
    char local_28;
    char local_27;
    char local_24[4];
    char local_20;
    char local_1e[5];
    char *local_18;

    local_18 = param_1;
    sVar2 = strlen(param_1);
    if (sVar2 == 0x1d) {  // Check if the length of the input string is 29 (0x1d in hex)
        strncpy(local_1e, local_18, 5);
        local_1e[5] = '\0';
        strncpy(local_24, local_18 + 6, 5);
        local_24[5] = '\0';
        strncpy(&local_2a, local_18 + 12, 5);
        local_2a = local_18[12];
        local_29 = local_18[13];
        local_28 = local_18[14];
        local_27 = local_18[15];
        strncpy(local_30, local_18 + 18, 5);
        local_30[5] = '\0';
        strncpy(local_36, local_18 + 24, 5);
        local_36[5] = '\0';

        if (local_18[5] == '-' && local_18[11] == '-' && local_18[17] == '-' && local_18[23] == '-') {
            local_3c = 0;
            local_40 = 0;
            for (local_44 = 0; local_44 < 5; local_44++) {
                pcVar3 = strchr("KJL", local_1e[local_44]);
                if (pcVar3 != NULL) {
                    local_3c++;
                }
                if (isupper(local_1e[local_44])) {
                    local_40++;
                }
            }
            if (local_3c == 1 && local_40 == 4 && isupper(local_20) && ((local_20 - '0') % 2 == 0) && isdigit(local_2a) && isdigit(local_29) && isupper(local_28) && isupper(local_27)) {
                iVar1 = abs(local_2a - local_29);
                if (iVar1 != (local_28 + local_27 - '0' * 2)) {
                    return 0;
                }
                for (local_50 = 0; local_50 < 5; local_50++) {
                    if (!isdigit(local_30[local_50])) {
                        return 0;
                    }
                }
                local_54 = 0;
                local_58 = 0;
                local_40 = 0;
                for (local_5c = 0; local_5c < 5; local_5c++) {
                    pcVar3 = strchr("HBTOP", local_36[local_5c]);
                    if (pcVar3 != NULL) {
                        local_54++;
                    }
                    if (isupper(local_36[local_5c])) {
                        local_40++;
                    }
                    if (isupper(local_36[local_5c]) && ((local_36[local_5c] - '0') % 2 != 0)) {
                        local_58++;
                    }
                }
                if (local_54 < 1 && local_58 != local_40) {
                    return 0;
                }
                return 1;
            }
        }
    }
    return 0;
}
```
So, back to fcn.00001980. It seems that we need the above function to return 1 and we need to make sure fcn.00001980 returns something nonzero as well.

There is a lot of code so I will just summarize what we need:
fcn.000011b0:
```bash
ulong FUN_001011b0(char *param_1,int param_2, ulong param_3) {
  ulong local_30;
  ulong local_28;
  char *local_10;
  
  local_28 = 0;
  local_30 = 1;
  for (local_10 = param_1; *local_10 != '\0'; local_10 = local_10 + 1) {
    local_28 = (local_28 + ((long)*local_10 * local_30) % param_3) % param_3;
    local_30 = (local_30 * (long)param_2) % param_3;
  }
  return local_28;
}
```
fcn.000018d0:
```bash
int FUN_001018d0(long param_1,long param_2) {
  int iVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  bool local_49;
  
  uVar4 = (ulong)(param_1 + param_2) % 0x3b9aca07;
  iVar1 = FUN_00101240(uVar4);
  local_49 = 0;
  if (iVar1 != 0) {
    uVar2 = FUN_00101430(0x3ff2,0x8003,0xc005);
    uVar2 = FUN_001012f0(uVar2);
    uVar3 = FUN_00101390(uVar2);
    if (uVar4 == uVar3) {
		local_49 = 1;
    }
  }
  return local_49;
}
```

fcn.00001430:
```bash
ulong FUN_00101430(ulong param_1,ulong param_2,ulong param_3)

{
  ulong local_28;
  ulong local_18;
  ulong local_10;
  
  local_28 = 1;
  local_10 = param_1;
  for (local_18 = param_2; local_18 != 0; local_18 = local_18 >> 1) {
    if ((local_18 & 1) != 0) {
      local_28 = (local_28 * local_10) % param_3;
    }
    local_10 = (local_10 * local_10) % param_3;
  }
  return local_28;
}
```
fcn.000012f0:
```bash
ulong FUN_001012f0(ulong param_1) {
  ulong uVar1;
  
  uVar1 = (param_1 ^ 0xa5a5a5a5a5a5a5a5) << 0xd;
  uVar1 = (uVar1 >> 0x11 | (uVar1 | (param_1 ^ 0xa5a5a5a5a5a5a5a5) >> 0x33) << 0x2f) ^
          0x5a5a5a5a5a5a5a5a;
  return ((uVar1 >> 0xb | uVar1 << 0x35) * 0x5851f42d4c957f2d) % 0xffffffffffffffff;
}
```
fcn.00001390:
```bash
ulong FUN_00101390(long param_1) {
  ulong uVar1;
  ulong uVar2;
  
  uVar2 = (ulong)(param_1 * -0x3f681078cd61d75b) % 0xffffffffffffffff;
  uVar2 = (uVar2 << 0xb | uVar2 >> 0x35) ^ 0x5a5a5a5a5a5a5a5a;
  uVar1 = uVar2 >> 0x2f;
  return ((uVar2 << 0x11 | uVar1) >> 0xd | uVar1 << 0x33) ^ 0xa5a5a5a5a5a5a5a5;
}
```
- fcn.000014b0 needs to return 1
- fcn.000011b0 is applied to both serial and username
- fcn.000018d0 needs to return 1

Now we can make a keygen:
