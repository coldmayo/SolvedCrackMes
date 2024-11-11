# What does the code do?

## main()

It seems the first thing the code does is call a function generate_password() which has arguements of: "sexy1337" and a char array.

It then calls another function called encrypt_decrypt that takes 4294967210 and the same char array as before.

It will then prompt the user to enter a password. Then it runs the encrypt_decrypt function on the entered string and then does a strcmp of the result of that with the one before.

## generate_password()

This function seems pretty simple.

There are two arguements, both are strings.

First a strlen is called on the 2nd parameter and a for loop is done that goes through up until the value of the strlen. In it there is the following:
```bash
param_1[i] = param_2[i] + '\x03';
```
After the for loop it sets the final value to zero.

Essentially, it seems to shift the contents of param_2 by 3 and then save it to param_1

## encrypt_decrypt()

This function only contains a for loop where:
```bash
for (int i = 0; param_1[i] != '\0'; i++) {
	param_1[i] = param_1[i] ^ param_2
}
```

So, it seems that an XOR is done on every char of param_1 by param_2

# Keygen

It seems all we need to do is to recreate and run the generate password function to get the password.

Below is the keygen.c file I made
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char * string = "sexy1337";
	int num = 4294967210;
	char buff[56];
	
	for (int i = 0; strlen(string) > i; i++) {
		buff[i] = string[i] + '\x03';
	}

	printf("%s\n", buff);
}
```

You should get a password of vh{|466: which worked for me.
