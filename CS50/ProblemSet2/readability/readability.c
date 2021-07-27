#include <ctype.h>
#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// CONSTATS:
static string TEXT;

// FUNCTIONS:
int calculateReadability(string text);
bool isPunctEnd(char c);
void handleGrade(int g);

int main(void)
{
    TEXT = get_string("Text: "); // get input text from a user

    handleGrade(calculateReadability(TEXT));
}

// calculates text readability based on Coleman-Liau index
int calculateReadability(string text)
{
    int letterCount = 0;
    int wordCount = 1; // probably write it as float to avod typecasting
    int sentenceCount = 0;

    // go through the text symbol by symbol until the end
    for (int i = 0; true; i++)
    {
        char c = text[i];

        if (c == '\0') // do it until the end of the string
        {
            break;
        }
        else
        {
            if (isalpha(c))     // count number of letters
            {
                letterCount++;
            }

            if (isblank(c))     // count number of words
            {
                wordCount++;
            }

            if (isPunctEnd(c))  // count number of sentences
            {
                sentenceCount++;
            }
        }
    }

    // Probably it's better to avoid typecasting
    float L = letterCount / (float) wordCount * 100;   // letters per 100 words
    float S = sentenceCount / (float) wordCount * 100; // sentences per 100 words

    // Coleman-Liau formula to calculate readability of a text
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    return index;
}

// checkes wheather or not char is a punctuation mark to end a sentence
bool isPunctEnd(char c)
{
    return c == '.' || c == '!' || c == '?';
}

// as it is
void handleGrade(int g)
{
    if (g < 0)
    {
        printf("Before Grade 1\n");
    }
    else if (g >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", g);
    }
}