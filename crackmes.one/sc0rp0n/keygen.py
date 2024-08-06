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
