#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// FUNCTIONS:
bool handleInput(int n, string s[]);
string toCipherText(string t);
char cipherHelper(int min, int max, int shift);

// Variables:
static int KEY; // probably poor design, the purpose is to avoid recomputation

// Purpose: filters loaded command line arguments;
//          gets user's text as input;
//          prints encrypted message;
int main(int argc, string argv[])
{
    if (handleInput(argc, argv))
    {
        string text = get_string("plaintext:  "); // requests plain text from a user
        string result = toCipherText(text);

        printf("ciphertext: %s\n", result);

        return 0; // exit code, no errors
    }
    else
    {
        printf("Usage: ./caesar key\n");

        return 1; // exit code, something went wrong
    }
}

// handles user's command line arguments
// Purpose: accepts only one positive integer
// Modifies: KEY
bool handleInput(int n, string s[])
{
    if (n == 2) // command contains only one argument
    {
        string t = s[1];

        for (int i = 0, r = strlen(t); i < r; i++) // checks whether every char in string is a number
        {
            if (!isdigit(t[i]))
            {
                return false; // string contains at least one non-digit i.e. "," or "." or even "-"
            }
        }

        KEY = atol(t) % 26; // key declaration

        return true; // command contains one argument and it is positive integer
    }

    return false; // command contains invalid amount of arguments
}

// Purpose: encrypt text with the key
// Modifies: text
// Requirements: KEY must be between 1 and 25
string toCipherText(string text)
{
    int textlen = strlen(text);

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        int shifted = c + KEY; //shifted letter

        if (isupper(c))
        {
            text[i] = cipherHelper(65, 90, shifted);
        }
        if (islower(c))
        {
            text[i] = cipherHelper(97, 122, shifted);
        }
        // if char is another symbol, it stays as is
    }
    return text;
}

// helper for cipher function
// Purpose: if shifted char is out of boundaries of ascii letters, function calculates right place for it
char cipherHelper(int min, int max, int shift)
{
    if (shift > max)
    {
        return (min - 1) + shift % max; // cringe formula, but it works <:P
    }
    else
    {
        return shift;
    }
}