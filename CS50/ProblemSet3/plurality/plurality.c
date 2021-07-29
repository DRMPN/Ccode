#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
// P.S: I'm not proud of this design, but that is what I came up with.
//      I blame initial bad design, how it is even possible to have more than one winner?
//      From my perspective, if few people have the same amount of votes, there should be another tour.
void print_winner(void)
{
    // initial state for winner, name should never be printed
    candidate winner;
    winner.name = "No winner";
    winner.votes = 0;

    // variables in case there might be more than one winner, which is absolutely trash idea >:(
    bool few_winners = false;
    int winner_count = 0;
    candidate winners[MAX];

    // iteration over all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        candidate c = candidates[i];

        if (c.votes > winner.votes)
        {
            winner.name = c.name;           // save new winner
            winner.votes = c.votes;

            winner_count = 0;               // if there were multiply winners, reset the list
            few_winners = false;            // signal that there's only one winner
        }

        if (c.votes == winner.votes)        // if candidate's votes are the same
        {
            winner.name = c.name;           // change winner's name
            winners[winner_count] = winner; // add another winner to the list of winners

            few_winners = true;             // signal that there's more than one winner
            winner_count++;                 // increase amount of winners
        }
    }

    // prints winner(s) name(s)
    if (few_winners)
    {
        for (int i = 0; i < winner_count; i++)
        {
            printf("%s\n", winners[i].name);
        }
    }
    else
    {
        printf("%s\n", winner.name);
    }
}
