# Requested Information

The following was extracted from the shell (times are from keygen compiled without optimization):

```
canada-geek@canada-geek $ time ./keygen
Valid key for user 'DZONERZY' is 'J0000-00000-AA000-0001I-7B38V'

real    7m32.344s
user    7m32.081s
sys     0m0.244s
canada-geek@canada-geek $ ./kgenmev2
*** Welcome to DZONERZY's keygenme v2! ***

------------------------------------
| Rules:                           |
|                                  |
| - Patching is not allowed!       |
| - Keygen gen time < 10 min       |
| - Comment with proof valid key   |
|   for username 'DZONERZY' and    |
|   result of commant 'time'       |
| - You have to write a keygen!    |
------------------------------------

Enter your username: DZONERZY
Enter your serial: J0000-00000-AA000-0001I-7B38V
[+] Serial is correct! Now go write a keygen
```

Just for fun, I also tried the key generator with a handful of other usernames (all of which pass) and also tried with optimization...

| Username | Serial | Time | Time (with optim) |
| - | - | - | - |
| DZONERZY | J0000-00000-AA000-0001I-7B38V | 07:32 | 04:01 |
| dzonerzy | J0000-00000-AA000-0001N-AXIX1 | 08:18 | 04:26 |
| abcdefghij | J0000-00000-AA000-00005-ZPBZ7 | 00:44 | 00:24 |
| canada-geek | J0000-00000-AA000-0000J-AYY3N | 02:42 | 01:28 |

# Analysis

The program reads 2 strings (`username` and `serial number`), then performs a series of hashing operations and eventually checks whether this `<username, serial number>` pair is acceptable.

## Serial Number Format

Before hashing, the serial number's format is verified. It needs to comply to the following template:

```
AAAAA-BBBBB-CCCCC-DDDDD-EEEEE
```

where:
- the whole serial number must be exactly 29 bytes
- it's composed of 5 5-character segments, separated by hyphen (`-`)
- each segment has specific conditions that need to be met...
- AAAAA:
    - must have exactly 1 character that is `J`, `K` or `L`
    - must have exactly 4 digits (`0` through `9`)
    - the position of each of these is not important
- BBBBB:
    - the first 4 characters have no restrictions
    - the 5th character must be an even digit (`0`, `2`, `4`, `6` or `8`)
- CCCCC:
    - the first 2 characters must be letters (alphabetic)
    - the next 2 characters must be digits (`0` through `9`)
    - the 5th character has no restrictions
    - the difference between the 2 alphabetic characters must equal
    the sum of the numbers represented by the 2 digits:
        - `abs(alpha[0] - alpha[1]) == digit[0] + digit[1] - 0x60`
        - (the `- 0x60` converts the digits from ASCII to numbers)
        - example: `AE31X`: letter difference is 4, digit sum `3+1 = 4`
- DDDDD:
    - all 5 digits must be alphanumeric
- EEEEE:
    - either:
        - at least one character must be `B`, `H`, `O`, `P`, or `T`
        - remaining characters have no restrictions
        - position of the character doesn't matter
    - or:
        - any/all numeric digits must be odd

## Hash Computation and Validation

Hash calculation is done the same way for the username and the serial number. The parameters that control how the hash is computed (the salt and the modulus) are hardcoded to `0x1f = 31` and `0x3b9aca07 = 1000000007` respectively [note that both are prime numbers]. The following pseudo-code (using python syntax) shows the hashing algorithm:

```python
def compute_hash(string, salt=0x1f, mod=0x3b9aca07):
    hash = 0
    temp = 1
    for char in string:
        hash = (hash + (char * temp) % mod) % mod
        temp = (temp * salt) % mod
    return hash
```

Once the two hashes are obtained, they are combined as follows (the modulus is the same value as above):

```python
combined_hash = (username_hash + serial_number_hash) % 0x3b9aca07
```

At this point in the program, there is an additional validation step which (it turns out) is not strictly necessary (see the next paragraphs).

The program then proceeds to compute a value using 3 complicated functions; however, the inputs to these functions are hard-coded in the program, so the outcome of these functions is always the same. Using a debugger (and confirming via test program), this computed value is verified to be `0x9cf = 2511`.

The final check is whether `combined_hash == 0x9cf`, which renders the above additional validation unnecessary (obviously if combined_hash is `0x9cf`, that additional validation function will always succeed, so its presence in the program is somewhat moot).

# Key Generator Design

Given the pattern that needs to be followed for the serial number, the total combinations are way too high for a program to compute a serial number for each iteration (a form of brute force).

The submitted keygen still uses brute force, but does so without recalculating the strings every time.

Instead, it precomputes all permutations of the 5 segments (A, B, C, D, E) separately and stores the strings into 5 arrays (this uses up about ~600 MiB).

Once these are computed, the program can iterate over _indexes_ into these arrays.  The hash calculation function can then be modified slightly so it picks up bytes from these 5 locations (plus the hyphens) instead of copying them or picking them up from a linear string.

The result of this is an algorithm that is capable of meeting the time constraints mentioned in the problem statement (less than 10 minutes to find a working serial number for a given username).

The key generator was written in C, and compiled using only basic options:

```bash
# Without optimization
gcc -Wall -Werror -g -o keygen keygen.c

# With optimization
gcc -O3 -Wall -Werror -g -o keygen keygen.c
```

Other things I could have tried:

- multithreading (right now it's single-threaded)
- I limited the serial number to uppercase letters and numbers only; strictly speaking, any 8-bit values (other than NUL) could be considered as candidates, at least for the parts that don't have restrictions
