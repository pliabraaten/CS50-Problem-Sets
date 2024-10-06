#include <cs50.h>
#include <stdio.h>

void Luhn(long card);

int main(void)
{
    // Prompt user for card number excluding hyphens
    long card;
    do
    {
        card = get_long("Number: ");
    }
    while (card < 1);

    long digits = card / 10000000000000;
    printf("%ld\n",digits);
    }
