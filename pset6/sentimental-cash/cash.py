
from cs50 import get_float

# Variables
quarter = 25
dime = 10
nickel = 5
penny = 1
coins = 0

# Prompt user for change owed
while True:
    change = get_float("Change: ")
    if change >= 0:
        break

# Convert to integer
change = change * 100

# Max number of quarters
while change / quarter >= 1:
    change = change - quarter
    coins += 1

# Max number of dimes
while change / dime >= 1:
    change = change - dime
    coins += 1

# Max number of nickels
while change / nickel >= 1:
    change = change - nickel
    coins += 1

# Max number of pennies
while change / penny >= 1:
    change = change - penny
    coins += 1

# Print minimum number of coins
print(f"{coins}")
