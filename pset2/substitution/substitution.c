#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char substitute(char text, string key);

int main(int argc, string argv[])
{
    // Ensure single command line argument
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Ensure key has 26 characters
    int key_len = strlen(argv[1]);
    if (key_len != 26)
    {
        printf("Key must contain 26 alphabetic characters\n");
        return 1;
    }
    // Ensure key only contains alphabetic characters
    for (int i = 0; i < key_len; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain alphabetic characters\n");
            return 1;
        }
    }
    // Ensure key contains unique characters
    for (int i = 0; i < key_len; i++)
    {
        for (int j = i + 1; j < key_len; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must contain 26 unique alphabetic characters\n");
                return 1;
            }
        }
    }
    // Define key
    string key = argv[1];

    // Prompt user for plaintext
    string plaintext = get_string("plaintext: ");

    // Print out encrypted message
    int text_len = strlen(plaintext);
    printf("ciphertext: ");
    // Loop through each character and substitute it using the key
    for (int i = 0; i < text_len; i++)
    {
        printf("%c", substitute(plaintext[i], key));
    }
    printf("\n");
    return 0;
}

char substitute(char c, string key)
{
    char cipher_letter;
    if (isalpha(c))
    {
        // If c from plaintext is uppercase
        if (isupper(c))
        {
            cipher_letter = toupper(key[c - 65]);
            return cipher_letter;
        }
        // If lowercase, return value from key in lowercase
        else
        {
            cipher_letter = tolower(key[c - 97]);
            return cipher_letter;
        }
    }
    // If plaintext character is not alphabetic, just print it
    else
    {
        return c;
    }
}
