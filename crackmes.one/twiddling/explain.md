So, we'll start by looking at main:
```bash
lea rax, [0x00002120]
mov rdi, rax
call sym.imp.puts
mov rdx, qword [obj.stdin]   # "Enter flag (e.g. flag{3x4mpl3_fl4g})"
lea rax, [s]
mov esi, 0x21
mov rdi, rax
call sym.imp.fgets
lea rdx, [var_b0h]
```
This snipet of code takes in user input after printing "Enter flag (e.g. flag{3x4mpl3_fl4g})"
```bash
lea rax, [s]
mov rsi, rdx
mov rdi, rax
call sym.stod
```
Seems to feed the user input and a int array (var_b0h). We'll come back to that.
```bash
mov dword [var_b4h], 0
lea rax, [s]
mov rdi, rax
call sym.imp.strlen
cmp rax, 0x20
jne 0x179d
```
This seems to find the length of user input and then if it is equal 32 then the jump does not occur. It would jump to here:
```bash
cmp dword [var_b4h], 0x20
jne 0x17bc
lea rax, str.You_did_it__What_a_good_reverser_you_are____n   # You did it! What a good reverser you are ;)\n
mov rdi, rax
mov eax, 0
call sym.imp.printf
jmp 0x17d0
```
If var_b4h is equal 32 then the jump does not occur. It seems we do not want it to occur because then we get the good boy prompt.

Otherwise we get "I'm sorry little one".

So, it seems that we need to make sure the first if statement makes ```var_b4``` ends up equaling 32 to trigger the second one.

But anyway, lets delve deeper into the first if statement:
```bash
lea rax, [var_b0h]
mov rdi, rax
call sym.this_is_where_the_fun_begins
# the following code is in C instead of assembly
for (int i = 0; i < 32; i++) {
	if (var_b0h[i] == 0x20a0+i*4) {
		var_b4++;
	}
}
```
Interesting, lets look at stod:
## stod

It seems this function takes the form of:
```bash
for (int i = 0; i < 32; i++) {
	param_2[i] = (int)param_1[i];
}
return;
```

Seems this function converts param_1 which is a character array to a corresponding integer array.

## this_is_where_the_fun_begins

Now we will go through the this_is_where_the_fun_begins function. I will write this in C since it is long
```bash
int num;
int other;
int other2;
for (int i = 0; i < 4; i++) {
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i * 8 + 2) * 4) * 4, (0x2020 + (i * 8 + 7) * 4) * 4 + param_1);
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i * 8 + 4) * 4) * 4, (0x2020 + (i * 8 + 2) * 4) * 4 + param_1);
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i << 3) * 4) * 4, 0x2020 + (i * 8 + 3) * 4) * 4 + param_1);
	for (int j = 0; j < 8; j++) {
		num = param_1 + (0x2020 + (j + i * 8)*4)*4;
		other = parry_this_you_casual(num);
		if (other == 0) {
			you_turn_my_world_around(param_1 + (0x2020 + (i + j * 8) * 4) * 4);
		} else {
			other2 = mr_krabs_likes_his_money(num);
			num = num ^ other2;
		}
		num = (0x2020 + (i + j * 8) * 4) * 4;
	}
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i * 8 + 1) * 4) * 4, (0x2020 + (i * 8 + 3) * 4) * 4 + param_1);
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i * 8 + 7) * 4) * 4, (0x2020 + (i * 8 + 6) * 4) * 4 + param_1);
	ive_never_been_stung_by_a_wasp(param_1 + (0x2020 + (i * 8 + 5) * 4) * 4, (0x2020 + (i * 8 + 1) * 4) * 4 + param_1);
}
return;
```

Seems to be an encoding function, but now we have more work to do. We need to find out what ive_never_been_stung_by_a_wasp, you_turn_my_world_around, mr_krabs_likes_his_money, and parry_this_you_casual. I think we can say that the outer loop confirms that the first characters are 'flag'.

## ive_never_been_stung_by_a_wasp

```bash
void (int * param_1, int * param_2) {
	*param_1 = *param_1 ^ *param_2;
	*param_2 = *param_2 ^ *param_1;
	*param_1 = *param_1 ^ *param_2;
	return;
}
```

## parry_this_you_casual
```bash
int parry_this_you_casual(int param_1) {
	int i;
	int j = 0;
	for (i = param_1; i != 0; i = i & i - 1) {
		j = j ^ 1;
	}
	return j;
}
```

## you_turn_my_world_around

```bash
void you_turn_my_world_around(int * param_1) {
	int i = 0;
	while (*param_1 != 0) {
		i = i << 1 | *param_1 & 1;
		*param_1 = (int)*param_1 >> 1;
	}
	*param_1 = i;
	return;
}
```

## mr_krabs_likes_his_money
```bash
int mr_krabs_likes_his_money(int param_1) {

	int i;
	int j;
	
	for (i = param_1; i != 0; i = (int)i >> 1) {
		j = j + (i & 1);
	}
	
	return j;
}
```

Ok, now we need to make a proper keygen, which can be found in keygen.c

I used a purely brute force method which was cringe. Eventually, I got flag{1w1DL1h6_1h3_b1h4Ry_5y513m}

