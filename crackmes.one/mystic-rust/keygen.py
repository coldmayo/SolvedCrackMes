def encode_input(key, num):
    buffer = []
    for i in range(len(key)):
        if (i & 1) == 0:
            if (ord(key[i]) & 1) == 0:
                buffer.append(chr(ord(key[i]) - num))
            else:
                buffer.append(chr(ord(key[i]) + num))
        else:
            if (ord(key[i]) & 1) == 0:
                buffer.append(chr(ord(key[i]) + num * 2))
            else:
                buffer.append(chr(ord(key[i]) + num * (-2)))
    buffer.append('\0')
    return ''.join(buffer)

# Check through numbers and strings
for num in range(1, 35):
    test_string = "ABCDEFGHIJKL"
    encoded = encode_input(test_string, num)
    if encoded[-2] == '|':  # '\0' is the last character, so check the second last
        print(f"String: {test_string}, Number: {num}, Encoded: {encoded}")
        break

