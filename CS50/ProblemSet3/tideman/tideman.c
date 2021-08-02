#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int calculate_pair_strength(int candidate);
bool find_cycle(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int n = i + 1; n < candidate_count; n++)
        {
            preferences[ranks[i]][ranks[n]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int n = i + 1; n < candidate_count; n++)
        {
            int c_a = preferences[i][n]; // voices of candidate a
            int c_b = preferences[n][i];

            if (c_a > c_b)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = n;
                pair_count++;
            }

            if (c_a < c_b)
            {
                pairs[pair_count].winner = n;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            // if two candidates have the same amount of voices, just skip
        }
    }
}

// Sort pairs in decreasing order by strength of victory
// simple algorithm, I may code it better, but too lazy for that
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int max = calculate_pair_strength(i);
        int max_pos = i;

        for (int n = i + 1; n < pair_count; n++)
        {
            int p_max = calculate_pair_strength(n); // possible maximum

            if (max < p_max) //  if current maximum is less than possible maximum
            {
                max = p_max; //  update maximum
                max_pos = n; //  update position of maximum
            }
        }

        pair temp_low = pairs[i];
        pair temp_high = pairs[max_pos];

        pairs[i] = temp_high;
        pairs[max_pos] = temp_low;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;

        locked[w][l] = true;

        // algorithm doesn't work
        if (find_cycle()) // if there's a cycle, revert changes
        {
            locked[w][l] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_connections = false;

        for (int n = 0; n < candidate_count; n++)
        {
            if (locked[n][i])
            {
                has_connections = true;
                break;
            }
        }

        if (!has_connections)
        {
            printf("%s\n", candidates[i]);
        }
    }
}

int calculate_pair_strength(int candidate)
{
    int w = pairs[candidate].winner; // get candidate position in the array
    int l = pairs[candidate].loser;

    int w_v = preferences[w][l]; // get voices for candidate w against l
    int l_v = preferences[l][w];

    return w_v - l_v;
}

bool find_cycle(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int n = i + 1; n < candidate_count; n++)
        {
            bool c_a = locked[i][n];
            bool c_b = locked[n][i];

            if (c_a && c_b)
            {
                return true;
            }
        }
    }

    return false;
}

bool find_cycle_new(void)
{
    return true;
}

/*
debug preferences

    printf("Candidates: %i\n", candidate_count);

    for (int i = 0; i < candidate_count; i++)
    {
        printf("Candidate: %i. ", i);
        printf("Connections: ");
        for (int n = 0; n < candidate_count; n++)
        {
            printf("%i ", preferences[i][n]);
        }
        printf("\n");
    }

debug pairs

    for (int i = 0; i < pair_count; i++)
    {
        printf("Pair %i:\n", i);
        printf("Winner: %s. ", candidates[pairs[i].winner]);
        printf("Loser: %s.\n", candidates[pairs[i].loser]);
    }

debug sort pairs

    printf("Pair: %i\n", n);
    printf("WinVoices: %i, LosVoices: %i\n", w_v, l_v);

*/

