from cs50 import get_int

# Prompt for height
while True:
    try:
        height = int(input("Height: "))
        if height > 0 and height != 9:
            break
    except ValueError:
        print("That's not an integer")

width = 1

# Print pyramid
for h in range(height):
    print((" " * (height - width)) + ("#" * width), end="  ")
    print("#" * width, end="\n")
    width += 1