
height = input("Height: ")

while not height or not height.isdigit() or not 1 <= int(height) <= 8:
    height = input("Height: ")

height = int(height)
for h in range(height):

    # Left bricks
    spaces = 0
    while spaces < height - h - 1:
        print(" ", end="")
        spaces += 1

    for bricks in range(h + 1):
        print("#", end="")

    # Spaces between sides
    print("  ", end="")

    # Right bricks
    for bricks in range(h + 1):
        print("#", end="")

    # Move to next line
    print("")
