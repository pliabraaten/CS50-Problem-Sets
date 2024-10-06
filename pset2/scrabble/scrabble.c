#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void convert_uppercase(string player1_word, string player2_word);
int score_word(string player1_word, string player2_word);

int score1 = 0;
int score2 = 0;
char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                     'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    // Prompt user for 2 words
    char *player1_word = (get_string("Player 1: "));
    char *player2_word = get_string("Player 2: ");

    // Convert letters to uppercase
    convert_uppercase(player1_word, player2_word);

    // Compute word scores
    score_word(player1_word, player2_word);

    // Compare points and print winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins\n!");
    }
    else if (score1 == score2)
    {
        printf("Tie!\n");
    }
}

// Convert all the letters to uppercase for consistency
void convert_uppercase(string player1_word, string player2_word)
{
    // Loop through each string, converting each letter to uppercase
    for (int i = 0, length = strlen(player1_word); i < length; i++)
        player1_word[i] = toupper(player1_word[i]);
    for (int j = 0, length = strlen(player2_word); j < length; j++)
        player2_word[j] = toupper(player2_word[j]);
}

// Calculate scores and save as variables score1 and score2
int score_word(string player1_word, string player2_word)
{
    // Loop through each character of the player1's word
    for (int i = 0, length = strlen(player1_word); i < length; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (player1_word[i] == alphabet[j])
            {
                score1 += points[j];
            }
        }
    }
    // Loop through each character of the player2's word
    for (int i = 0, length = strlen(player2_word); i < length; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (player2_word[i] == alphabet[j])
            {
                score2 += points[j];
            }
        }
    }
    return score1;
    return score2;
}
