from cs50 import get_int

# Get number
while True:
    num = get_int("Number: ")
    number = [int(x) for x in str(num)]
# Check length
    if len(number) == 13:
        card = "VISA\n"
        break
    elif len(number) == 15:
        card = "AMEX\n"
        break
    elif len(number) == 16:
        if number[0] == 4:
            card = "VISA\n"
        else:
            card = "MASTERCARD\n"
        break
    else:
        print("INVALID\n")
        exit(0)

# Initialize Sum
sum = 0

# Luhn’s Algorithm
for x in range(int(round((len(number) / 2)))):
    if (len(number) % 2) == 0:
        start = 0
        sum += (number[1 + x * 2])
        hold = (number[start + x * 2]) * 2
    else:
        start = 1
        sum += (number[x * 2])
        if (start + x * 2) != len(number):
            hold = (number[start + x * 2]) * 2
    while hold > 0:
        rem = hold % 10
        sum += rem
        hold = int(hold / 10)

# Check if number ends in 0
if sum % 10 != 0:
    print("INVALID\n")
    exit(0)

# Print the card
print(card)