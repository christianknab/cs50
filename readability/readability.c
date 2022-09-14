#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

// Calculates approximate grade level based on the Coleman-Liau index formula
int main(void)
{
    // Asks for text input
    string text = get_string("Text: ");
    printf("%s\n", text);

    // Calculates number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculates Coleman-Liau index
    double index = 0.0588 * (letters / (double)words * 100) - 0.296 * (sentences / (double)words * 100) - 15.8;
    // Rounds index
    int gradeLevel = round(index);

    // Prints Grade level from the index
    if (gradeLevel < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (gradeLevel > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", gradeLevel);
    }
}

// Counts number of letters in the text
int count_letters(string text)
{
    int letters = 0;
    int length = strlen(text);

    // Counts every alphabetical letter
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

// Counts number of words in the text
int count_words(string text)
{
    int words = 1;
    int length = strlen(text);

    // Counts every spaces as a word
    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    return words;
}

// Counts number of sentences in the text
int count_sentences(string text)
{
    int sentences = 0;
    int length = strlen(text);

    // Counts every period, exclamation mark, and question mark
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}