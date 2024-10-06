
height = input("Height: ")

while not height or not height.isdigit() or not 1 <= int(height) <= 8:
    height = input("Height: ")

height = int(height)

for h in range(height):
    spaces = 0
    while spaces < height - h - 1:
        print(" ", end="")
        spaces += 1
    for brick in range(h + 1):
        print("#", end="")
    print("")
