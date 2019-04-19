from cs50 import get_int

#prompt user for appropraite height
while True:
    h = get_int("Height: ")
    if h >= 0 and h < 24:
        break

#for every row
for i in range(h):
    #print spaces
    print(" " * (h-i-1), end="")
    #print hashes & new line
    print("#" * (i+2))
