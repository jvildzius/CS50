from cs50 import get_string
import sys

if len(sys.argv) != 2:
    print("Please enter 1 command line argument")
    exit(1)

key = int(sys.argv[1])
s = get_string("plaintext: ")

print("ciphertext: ", end="")

for i in s:
    if i.isalpha():
        if i.islower():
            j = chr(((ord(i) - ord('a') + key) % 26) + ord('a'))
            print(j, end="")
        elif i.isupper():
            k = chr(((ord(i) - ord('A') + key) % 26) + ord('A'))
            print(k, end="")
    else:
        print(i, end="")

print()