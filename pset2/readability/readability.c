#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int ColemanLiau(float L, float S);
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for text
    string text = get_string("Text: ");

    // Count the number of words in the text
    int word_count = count_words(text);

    // Count the average number of letters in the text
    float L = (float) count_letters(text) / (float) word_count * 100;

    // Count the average number of sentences in the text
    float S = (float) count_sentences(text) / (float) word_count * 100;

    // Run Coleman-Liau index's reading level test
    int Grade = ColemanLiau(L, S);

    // Print reading level from "Before Grade 1" to "Grade 16+"
    if (Grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (Grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", Grade);
    }
}

// Count average number of letters per 100 words in text
int count_letters(string text)
{
    int letter_count = 0;
    // Loop through each element in the string
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Add to letter count if element is a letter
        if (isalpha(text[i]))
        {
            letter_count++;
        }
    }
    // printf("letters: %i\n", letter_count);
    return letter_count;
}

// Count number of words in the text
int count_words(string text)
{
    // Initialize word count at 1 since words = spaces + 1
    int word_count = 1;
    // Loop through each element in string looking for spaces
    for (int i = 0, len = strlen(text); i < len - 1; i++)
    {
        // Total number of spaces plus 1 for total word count
        if (isblank(text[i]) && !isblank(text[i + 1]))
        {
            word_count++;
        }
    }
    // printf("words: %i\n", word_count);
    return word_count;
}

// Count average number of sentences per 100 word in text
int count_sentences(string text)
{
    int sentence_count = 0;
    // Loop through each element looking for punctuation
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        // Count the number of punctuation for number of sentences
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
    }
    // printf("sent: %i\n", sentence_count);
    return sentence_count;
}

// Coleman-Liau index's reading level commutation
int ColemanLiau(float L, float S)
{
    float index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}
