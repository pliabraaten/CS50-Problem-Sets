#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>

bool Luhn(long fullcard, int digitcount);

int main(void)
{
    // Prompt user for card number excluding hyphens
    long fullcard;
    do
    {
        fullcard = get_long("Number: ");
    }
    while (fullcard < 1);

    // Check if first digits match with credit card code
    // Determine number of digits
    int digitcount = 0;
    long ncard = fullcard;
    while (ncard > 0)
    {
        digitcount = digitcount + 1;
        ncard = ncard / 10;
    }

    // Run checksum
    bool isValid = Luhn(fullcard, digitcount);

    // If Luhn's checksum is false then print invalid
    if (!isValid)
    {
        printf("INVALID\n");
    }
    else
    {
        // Check if card number is Mastercard (16-digits) or Visa (16 or [13 digits later])
        if (digitcount == 16)
        {
            // Check if card starts with 55 - Mastercard
            if (fullcard / 100000000000000 == 55)
            {
                printf("MASTERCARD\n");
            }

            // Check if card starts with 4 - Visa
            else if (fullcard / 1000000000000000 == 4)
            {
                printf("VISA\n");
            }

            // Loop to check if card starts with 51,52,52,or 53 - Mastercard
            else
            {
                bool masterstart = false;
                for (int i = 51; i < 54; i++)
                {
                    if (fullcard / 100000000000000 == i)
                    {
                        printf("MASTERCARD\n");
                        masterstart = true;
                    }
                }
                // If not 51-53 then print invalid
                if (masterstart == false)
                {
                    printf("INVALID\n");
                }
            }
        }
        // Check if card number is 13 digits - Visa
        else if (digitcount == 13)
        {
            // Check if card starts with 4 - Visa
            if (fullcard / 1000000000000 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        // Check if card number is 15-digits - Amex
        else if (digitcount == 15)
        {
            // Check if card starts with 34 or 37
            if (fullcard / 10000000000000 == 34 || fullcard / 10000000000000 == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
}

// Luhn’s algorithm
bool Luhn(long fullcard, int digitcount)
{
    long ncard = fullcard;
    int productsum = 0;
    // Loop through each digit place of card number starting at ones
    for (int i = 0; i < (digitcount); i++)
    {
        // Check if counter is even to find every other digit
        if (i % 2 == 0)
        {
            // Isolate each digit
            int evendigit = ((ncard % 100) / 10);

            // Multiply digit by 2
            int product = evendigit * 2;

            // If the product is 2 digits sum digits, otherwise sum
            if (product >= 10)
            {
                productsum += (product % 10) + (product / 10);
            }
            else
                productsum += product;
        }
        // Drop last digit to continue process
        ncard = ncard / 10;
    }

    // Find and sum the odd digits of the card number
    long card = fullcard;
    int checksum = productsum;
    bool isValid;
    int odddigit;

    // Loop through digits for full card again
    for (int i = 0; i < digitcount; i++)
    {
        // Isolate each odd digit
        odddigit = card % 10;

        // Add odd digit to sum
        checksum += odddigit;

        // Drop last 2 digits to continue process with odd numbers
        card = card / 100;
    }
    // Check if final check sum ends in 0
    if (checksum % 10 == 0)
    {
        isValid = true;
    }
    else
    {
        isValid = false;
    }
    return isValid;
}
