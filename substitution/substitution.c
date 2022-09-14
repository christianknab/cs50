#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

// Define alphabet
string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string ciphertext(string plaintext, string cipher);

// Main method
int main(int argc, string argv[])
{
    int KEY_LENGTH = 26;
    // Check if multiple arguments are passed
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check if the length of the argument is 26 letters long
    else if (strlen(argv[1]) != KEY_LENGTH)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // Check for duplicates and alpha characters
    else
    {
        string cipher = argv[1];
        // Iterates through the cipher
        for (int i = 0; i < 26; i++)
        {
            for (int k = i + 1; k < 26; k++)
            {
                // Checks for duplicates
                if (cipher[i] == cipher[k])
                {
                    printf("Key must not have duplicates.\n");
                    return 1;
                }
                // Checks for alpha character
                if (false == isalpha(cipher[i]))
                {
                    printf("Key must only use letters from the alphabet.\n");
                    return 1;
                }
            }
        }
    }

    // Get plaintext input
    string plaintext = get_string("plaintext:  ");
    // Provide the ciphertext
    printf("ciphertext: %s\n", ciphertext(plaintext, argv[1]));
}

// Converts the plaintext to ciphertext
string ciphertext(string plaintext, string cipher)
{
    // Initialize variables
    int lengthText = strlen(plaintext);
    int lengthAlpha = strlen(ALPHABET);
    string ciphertext = plaintext;

    // Iterates through the plaintext
    for (int i = 0; i < lengthText; i++)
    {
        // Sets variable 'letter' to each letter in plaintext one by one
        char letter = plaintext[i];

        // Iterates through the alphabet to find the index position of 'letter'
        for (int k = 0; k < lengthAlpha; k++)
        {
            // Checks if the 'letter' is found in the alphabet
            if (letter == ALPHABET[k] || toupper(letter) == ALPHABET[k])
            {
                // Changes the plaintext 'letter' to corresponding cipher letter, case sensitive
                if (islower(letter))
                {
                    ciphertext[i] = tolower(cipher[k]);
                }
                else if (isupper(letter))
                {
                    ciphertext[i] = toupper(cipher[k]);
                }
                break;
            }
        }
    }
    return ciphertext;
}