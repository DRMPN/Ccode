#include <stdio.h>
#include <cs50.h>

// FUNCTIONS:
int getAppropriateSize(int a, string b);
int nextYearPopulation(int a);
int calculatePopulationEnding(int a, int b);

// CONSTANTS:
int minPopSize = 9;
string getStartSizeText = "Enter start size: ";
string getEndSizeText = "Enter end size: ";


int main(void)
{
    int startSize = getAppropriateSize(minPopSize, getStartSizeText);
    int endSize = getAppropriateSize(startSize, getEndSizeText);

    printf("Years: %i\n", calculatePopulationEnding(startSize, endSize));
}


// first argument is minimum size, second argument is displayed text
int getAppropriateSize(int ls, string text)
{
    int n;
    do
    {
        n = get_int("%s", text);
    }
    while (n < ls);
    return n;
}

// produces population of the next year
int nextYearPopulation(int currPop)
{
    int newPop = currPop + (currPop / 3) - (currPop / 4);
    return newPop;
}

// produces number of years required to reach requisite population size
int calculatePopulationEnding(int startPop, int endPop)
{
    int n;
    for (n = 0; startPop < endPop; n++)
    {
        startPop = nextYearPopulation(startPop);
    }
    return n;
}