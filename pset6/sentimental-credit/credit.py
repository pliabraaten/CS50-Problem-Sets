
checksum = 0
sum_odd_digits = 0
sum_even_digits = 0

credit_card = input("Number: ")

# Convert card number to list of digits
digits = [int(digit) for digit in credit_card]

# Validate based on Luhn’s Algorithm

# Even digits
# Loop every other digit right to left starting with second to last digit
# [start: end: step]
for n in digits[-2::-2]:
    if n * 2 >= 10:
        sum_even_digits += (n * 2 % 10) + (n * 2 // 10)
    else:
        sum_even_digits += n * 2

# Odd digits
for n in digits[-1::-2]:
    sum_odd_digits += n

# Add even digit products and odd digits sum
if (sum_even_digits + sum_odd_digits) % 10 != 0:
    print("INVALID")
elif len(credit_card) not in [13, 15, 16]:
    print("INVALID")
elif len(digits) == 15 and digits[0] == 3 and (digits[1] in [4, 7]):
    print("AMEX")
elif len(digits) == 16 and digits[0] == 5 and (digits[1] in [1, 2, 3, 4, 5]):
    print("MASTERCARD")
elif len(digits) == 13 or 16 and digits[0] == 4:
    print("VISA")
else:
    print("INVALID")
