from cs50 import get_float

#include <math.h>

while True:
    change = get_float("Change owed: ")
    if change > 0:
        break
print(f"Change owed: {change}")

#convert dollars to cents
cents = round(change * 100)
print(f"{cents}")
count = 0

count += cents // 25
count += (cents % 25) // 10
count += ((cents % 25) % 10) // 5
count += ((cents % 25) % 10) % 5

print(f"Count: {count}")
