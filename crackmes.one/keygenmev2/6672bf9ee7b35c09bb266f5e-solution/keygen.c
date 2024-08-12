#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//////////////////////////////////////
// SERIAL NUMBER SEGMENT GENERATION //
//////////////////////////////////////
//
// Serial number is formatted as AAAAA-BBBBB-CCCCC-DDDDD-EEEEE.
//
// Each lettered segment has its own rules for valid character
// combinations.
//
// All permutations of each segment are generated (separately)
// into global variables aaaaa, bbbbb, ccccc, ddddd, eeeee,
// respectively.
//
// These are then referenced (without recomputation or copying)
// while performing hash calculation or verifying whether the
// serial number is valid for the given username.
//
// Note that each string within the arrays is of width 6, so
// there is always a trailing NUL character.  If memory is at a
// premium, this could be trimmed down to 5, as long as care is taken
// not to exceed the 5 characters in each entry.
//
// For now, stick to uppercase letters and numbers.

enum {
    SERIAL_LEN = 29
};

/*
 * Rules for AAAAA:
 *
 * - exactly one of JKL
 * - exactly 4 digits
 */
enum {
    AAAAA_COUNT = 3 * 5 * 10000,
};

static char aaaaa[AAAAA_COUNT][6];

void
aaaaa_gen()
{
    int aix, pos, i;
    char digits[4] = { '0', '0', '0', '0' };
    char jkl = 'J';

    memset(aaaaa, 0, sizeof(aaaaa));
    aix = 0;

    char* parts[5] = { 0, 0, 0, 0, 0 };

    for (pos = 0; pos < 5; ++pos) {
        // Set up 5 char pointers for preparing value.
        int dpos = 0;
        for (i = 0; i < 5; ++i) {
            if (i == pos) {
                parts[i] = &jkl;
            } else {
                parts[i] = &digits[dpos++];
            }
        }

        do {
            do {
                char* here = aaaaa[aix++];
                for (i = 0; i < 5; ++i) {
                    here[i] = *parts[i];
                }

                for (i = 4; i--;) {
                    if (digits[i] < '9') {
                        digits[i]++;
                        break;
                    }
                    digits[i] = '0';
                }
            } while(i >= 0);

            if (jkl < 'L') {
                jkl++;
            } else {
                jkl = 'J';
                break;
            }
        } while(1);
    }

    assert(aix == AAAAA_COUNT);
}

/*
 * Rules for BBBBB:
 *
 * - the first four characters can be any character
 * - the fifth character must be an even digit
 */
enum {
    BBBBB_COUNT = 36 * 36 * 36 * 36 * 5,
};

static char bbbbb[BBBBB_COUNT][6];

void
bbbbb_gen()
{
    int bix, i;
    char chars[5] = { '0', '0', '0', '0', '0' };

    memset(bbbbb, 0, sizeof(bbbbb));
    bix = 0;

    do {
        memcpy(bbbbb[bix++], chars, 5);

        for (i = 5; i--;) {
            if (i == 4) {
                if (chars[i] < '8') {
                    chars[i] += 2;
                    break;
                }
                chars[i] = '0';
            } else {
                if (chars[i] < '9') {
                    chars[i]++;
                    break;
                }
                if (chars[i] == '9') {
                    chars[i] = 'A';
                    break;
                }
                if (chars[i] < 'Z') {
                    chars[i]++;
                    break;
                }
                chars[i] = '0';
            }
        }
    } while(i >= 0);

    assert(bix == BBBBB_COUNT);
}

/*
 * Rules for CCCCC:
 *
 * - the first 2 characters must be alphabetic
 * - the next 2 characters must be numeric
 * - the last character has no restrictions
 * - abs(C[0] - C[1]) == C[2] + C[3] - 0x60
 *
 * The RHS of this expression is simply the sum of the numbers
 * represented by the two digits (e.g. '2' + '3' - 0x60 == 5).
 *
 * So the largest difference between the C[0] and C[1] is 18
 * (which is the sum of '9' + '9' - 0x60).
 */
enum {
    CCCCC_COUNT = 121464,
};

static char ccccc[CCCCC_COUNT][6];

void
ccccc_gen()
{
    int cix;
    char chars[3]  = { 'A', 'A', '0' };
    char digits[2] = { '0', '0' };

    memset(ccccc, 0, sizeof(ccccc));
    cix = 0;

    do {
        char* here = ccccc[cix++];
        here[0] = chars[0];
        here[1] = chars[1];
        here[2] = digits[0];
        here[3] = digits[1];
        here[4] = chars[2];

        if (chars[2] < '9') {
            chars[2]++;
            continue;
        }
        if (chars[2] == '9') {
            chars[2] = 'A';
            continue;
        }
        if (chars[2] < 'Z') {
            chars[2]++;
            continue;
        }
        chars[2] = '0';

        do {
            if (digits[1] < '9') {
                digits[1]++;
                continue;
            }
            digits[1] = '0';
            if (digits[0] < '9') {
                digits[0]++;
                continue;
            }
            digits[0] = '0';
            if (chars[1] < 'Z') {
                chars[1]++;
                continue;
            }
            chars[1] = 'A';
            if (chars[0] < 'Z') {
                chars[0]++;
                continue;
            }
            chars[0] = 'A';
        } while(abs(chars[0] - chars[1]) != digits[0] + digits[1] - 0x60);

        if (chars[0] == 'A' && chars[1] == 'A' &&
            digits[1] == '0' && digits[1] == '0') {
            break;
        }
    } while(1);

    assert(cix == CCCCC_COUNT);
}

/*
 * Rules for DDDDD:
 *
 * - all digits must be alphanumeric
 */

enum {
    DDDDD_COUNT = 36 * 36 * 36 * 36 * 36,
};

static char ddddd[DDDDD_COUNT][6];

void
ddddd_gen()
{
    int dix, i;
    char chars[5] = { '0', '0', '0', '0', '0' };

    memset(ddddd, 0, sizeof(ddddd));
    dix = 0;

    do {
        memcpy(ddddd[dix++], chars, 5);

        for (i = 5; i--;) {
            if (chars[i] < '9') {
                chars[i]++;
                break;
            }
            if (chars[i] == '9') {
                chars[i] = 'A';
                break;
            }
            if (chars[i] < 'Z') {
                chars[i]++;
                break;
            }
            chars[i] = '0';
        }
    } while(i >= 0);

    assert(dix == DDDDD_COUNT);
}

/*
 * Rules for EEEEE:
 *
 * - at least one character must be BHOPT
 * OR
 * - any digits must be odd
 */

enum {
    EEEEE_COUNT = 49787675,
};

static char eeeee[EEEEE_COUNT][6];

// There are 31 combinations of 1, 2, 3, 4 or 5 digits; they are
// hardcoded here to save on computation.
static const char* eeeee_digit_combos[] = {
    // 1 digit
    "0____", "_0___", "__0__", "___0_", "____0",

    // 2 digits
    "00___", "0_0__", "0__0_", "0___0",
    "_00__", "_0_0_", "_0__0",
    "__00_", "__0_0",
    "___00",

    // 3 digits
    "000__", "00_0_", "00__0", "0_00_", "0_0_0", "0__00",
    "_000_", "_00_0", "_0_00", "__000",

    // 4 digits
    "0000_", "000_0", "00_00", "0_000", "_0000",

    // 5 digits
    "00000"
};

#define NUM_COMBOS ( sizeof(eeeee_digit_combos) \
                   / sizeof(eeeee_digit_combos[0]) )

void
eeeee_gen()
{
    int eix, pos, i;
    char chars[5] = { '0', '0', '0', '0', '0' };
    char bhopt = 'B';

    memset(eeeee, 0, sizeof(eeeee));
    eix = 0;

    char* parts[5] = { 0, 0, 0, 0, 0 };

    // Part 1: BHOPT

    for (pos = 0; pos < 5; ++pos) {
        // Set up 5 char pointers for preparing value.
        int dpos = 0;
        for (i = 0; i < 5; ++i) {
            if (i == pos) {
                parts[i] = &bhopt;
            } else {
                parts[i] = &chars[dpos++];
            }
        }

        do {
            do {
                char* here = eeeee[eix++];
                for (i = 0; i < 5; ++i) {
                    here[i] = *parts[i];
                }

                for (i = 4; i--;) {
                    if (chars[i] < '9') {
                        chars[i]++;
                        break;
                    }
                    if (chars[i] == '9') {
                        chars[i] = 'A';
                        break;
                    }
                    if (chars[i] < 'Z') {
                        chars[i]++;
                        break;
                    }
                    chars[i] = '0';
                }
            } while(i >= 0);

            if (bhopt == 'T') {
                bhopt = 'B';
                break;
            }
            if (bhopt == 'P') {
                bhopt= 'T';
                continue;
            }
            if (bhopt == 'O') {
                bhopt = 'P';
                continue;
            }
            if (bhopt == 'H') {
                bhopt = 'O';
                continue;
            }
            assert(bhopt == 'B');
            bhopt = 'H';
        } while(1);
    }

    // Part 2: digits

    for (pos = 0; pos < NUM_COMBOS; ++pos) {
        const char* combo = eeeee_digit_combos[pos];

        // Initialize the chars.
        for (i = 0; i < 5; ++i) {
            if (combo[i] == '0') {
                chars[i] = '1';
            } else {
                chars[i] = 'A';
            }
        }

        do {
            memcpy(eeeee[eix++], chars, 5);

            for (i = 5; i--;) {
                if (combo[i] == '0') {
                    if (chars[i] < '9') {
                        chars[i] += 2;
                        break;
                    }
                    chars[i] = '1';
                } else {
                    if (chars[i] < 'Z') {
                        chars[i]++;
                        // Avoid characters that would lead to part-1
                        // permutations for EEEEE.
                        while(strchr("BHOPT", chars[i])) {
                            chars[i]++;
                        }
                        break;
                    }
                    chars[i] = 'A';
                }
            }
        } while(i >= 0);
    }

    assert(eix == EEEEE_COUNT);
}

void
serial_init()
{
    aaaaa_gen();
    bbbbb_gen();
    ccccc_gen();
    ddddd_gen();
    eeeee_gen();
}

typedef char (*segment_ptr)[6];

static const segment_ptr segments[5] = {
    aaaaa,
    bbbbb,
    ccccc,
    ddddd,
    eeeee
};

static const int seg_nums[5] = {
    AAAAA_COUNT,
    BBBBB_COUNT,
    CCCCC_COUNT,
    DDDDD_COUNT,
    EEEEE_COUNT
};


////////////////////////////////////////////////////
// SERIAL NUMBER HASH CALCULATION AND VERIFICATON //
////////////////////////////////////////////////////

#define HMOD 0x3b9aca07
#define TUPD 0x1f
#define HCMP 0x9cf

/*
 * Compute the hash for a string.
 *
 * There are 2 modes:
 * - if 'string' is non-null, it is iterated and hashed
 *   (used here to hash the username)
 * - if 'indices' is non-null, it contains 5 indices into the ABCDE
 *   segments where the strings for each segment can be extracted
 *   (used here to hash the serial number by reading segments without
 *   copying them)
 * - abort if neither or both of the pointers is null
 */
unsigned long
compute_hash(const char* string, const int* indices)
{
    assert((string && !indices) || (!string && indices));

    int length = string ? strlen(string) : SERIAL_LEN;

    unsigned long hash = 0;
    unsigned long temp = 1;
    for(int i = 0; i < length; ++i) {
        char c;
        if (string) {
            c = string[i];
        } else {
            int local_index = i % 6;
            if (local_index < 5) {
                int segno = i / 6;
                c = segments[segno][indices[segno]][local_index];
            } else {
                c = '-';
            }
        }

        hash = (hash + ((unsigned long)c * temp) % HMOD) % HMOD;
        temp = (temp * TUPD) % HMOD;
    }
    return hash;
}

/*
 * Check whether the current <user-hash, serial-number> pair is valid.
 *
 * The serial number is passed as an array of indices as explained
 * above.
 */
int
validate_hash(unsigned long user_hash, int* indices)
{
    unsigned long serial_hash = compute_hash(0, indices);
    return ((user_hash + serial_hash) % HMOD) == HCMP;
}


/////////////////////////////////////////
// HUNTING FOR A GOOD USER/SERIAL PAIR //
/////////////////////////////////////////

// For now, it exits after the first successful hit; it could easily
// be refactored to retrieve all user/serial pairs that work.

int
hunt_for_good_serial(const char* username, char* put_serial_here)
{
    int i, indices[5] = { 0, 0, 0, 0, 0 };

    unsigned long user_hash = compute_hash(username, 0);

    do {
        if (validate_hash(user_hash, indices)) {
            // Got a hit!!!
            snprintf(put_serial_here, SERIAL_LEN+1,
                     "%s-%s-%s-%s-%s",
                     aaaaa[indices[0]],
                     bbbbb[indices[1]],
                     ccccc[indices[2]],
                     ddddd[indices[3]],
                     eeeee[indices[4]]);
            return 1;
        }

        for(i = 5; i--;) {
            if (indices[i] < seg_nums[i] - 1) {
                indices[i]++;
                break;
            }
            indices[i] = 0;
        }
    } while(i >= 0);

    return 0;
}


/////////////////
// ENTRY POINT //
/////////////////

int
main(int argc, const char** argv)
{
    const char* username = "DZONERZY";

    if (argc > 1) {
        username = argv[1];
    }

    serial_init();

    char serial[SERIAL_LEN + 1];
    memset(serial, 0, sizeof(serial));

    int outcome = hunt_for_good_serial(username, serial);
    if (outcome) {
        printf("Valid key for user '%s' is '%s'\n", username, serial);
    }

    return !outcome;
}
