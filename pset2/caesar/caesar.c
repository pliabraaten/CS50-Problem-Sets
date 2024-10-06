#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);
char rotate(char c, int k);

int main(int argc, string argv[])
{
    // Get key from user in command line prompt
    // Make sure command line prompt only takes 1 argument
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Ensure command line argument only contains digits
    if (!only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert the 2nd command line argument to key
    int k = atoi(argv[1]);

    // Prompt user for plain text message
    string plaintext = get_string("plaintext: ");

    // Encipher text and print encrypted message
    printf("ciphertext: ");
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++)
    {
        printf("%c", rotate(plaintext[i], k));
    }
    printf("\n");
}

// Make sure command line argument only contains digits
bool only_digits(string s)
{
    int len = strlen(s);
    for (int i = 0; i < len; i++)
    {
        // If any element is not a digit (decimals or negative)
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Rotates single character by key amount, wrapping around
char rotate(char c, int k)
{
    if (isalpha(c))
    {
        if (isupper(c))
        {
            // Convert letter back to alphabetical index (0-26 value) by subtracting ASCII 65 offset
            // Divide by 26 and take the rem
            char cipher_letter = ((c - 65 + k) % 26) + 65;
            return cipher_letter;
        }
        else
        {
            char cipher_letter = ((c - 97 + k) % 26) + 97;
            return cipher_letter;
        }
    }
    else
    {
        return c;
    }
}
