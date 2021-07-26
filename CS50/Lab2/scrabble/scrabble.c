#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Funcitons
int compute_score(string word);
string compare_scores(int score1, int score2);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Compare two scores
    string result = compare_scores(score1, score2);

    // Print result
    printf("%s\n", result);
}

// Compute and return score for a string
int compute_score(string word)
{

    int sum = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {

        char c = word[i];

        if (isalpha(c)) // checks whether character is a letter or not
        {
            int t = tolower(c); // lowercasing a letter

            sum += POINTS[t - 97]; // get points from the array and adds it to word's sum
        }
    }

    return sum;
}

// funciton to compare 2 results and print it
string compare_scores(int score1, int score2)
{
    if (score1 > score2)
    {
        return "Player 1 wins!";
    }
    else if (score1 < score2)
    {
        return "Player 2 wins!";
    }
    else
    {
        return "Tie!";
    }
}
