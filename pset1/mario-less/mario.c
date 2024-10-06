#include <cs50.h>
#include <stdio.h>

void print_row(int spaces, int bricks);

int main(void)
{
    // prompt use for pyramid height
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    // print # until height is reached
    for (int i = 0; i < h; i++)
    {
        // print one row
        print_row(h - i - 1, i + 1);
    }
}
void print_row(int spaces, int bricks)
{
    // print spaces
    for (int s = 0; s < spaces; s++)
    {
        printf(" ");
    }
    // print bricks
    for (int l = 0; l < bricks; l++)
    {
        printf("#");
    }
    printf("\n");
}
