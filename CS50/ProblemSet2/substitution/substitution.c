#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Funcitons:
int verifyKey(string key);
string toCipherText(string text, string key);


int main(int argc, string argv[])
{
    if (argc != 2) // accepts only one command line argument
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    if (verifyKey(key)) // accepts only correct key
    {
        return 1;
    }

    string plainText = get_string("plaintext:  ");
    string cipherText = toCipherText(plainText, key);

    printf("ciphertext: %s\n", cipherText);

    return 0;
}

// Purpose: checks whether or not key contains only 26 letters and each letter exactly once
int verifyKey(string key)
{
    if (strlen(key) != 26) // accepts only 26 char long key
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    char lowerKey[26]; // consists of only lowercase letters. Purpose: ensure that a key holds against aLtErNaTiNg cAsE

    for (int i = 0; i < 26; i++)
    {
        char c = key[i];

        if (!isalpha(c)) // if char is not a letter, return an error
        {
            printf("Key must contain only letters.\n");
            return 1;
        }

        c = tolower(c);

        for (int n = i; n >= 0; n--) // probably bad efficiency aka nested loop, couldn't find better place for this check;
        {
            if (lowerKey[n] == c) // if char was already in the key, return an error
            {
                printf("Key must contain each letter exactly once.\n");
                return 1;
            }
        }

        lowerKey[i] = c;
    }

    return 0; // if key contains only letters and each letter exactly once, return good
}

// Purpose:  encrypts text with the key
// Modifies: text that was passed into the function
string toCipherText(string text, string key)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];

        if (islower(c))
        {
            text[i] = tolower(key[c - 97]); // converts letter to lowercase, because KEY might be in the different case
        }

        if (isupper(c))
        {
            text[i] = toupper(key[c - 65]); // same here
        }

        // if char is other than alphabetic letter it stays as is
    }

    return text;
}