#include <stdio.h>
#include <math.h>
#include <cs50.h>

// FUNCTIONS:
float getCorrectChange(void);
int dollarsToCents(float dollars);
int countCoins(int cents);

int main(void)
{
    int cents = dollarsToCents(getCorrectChange());

    printf("%i\n", countCoins(cents));
}

// recives only positive number from user
float getCorrectChange(void)
{
    float n;
    do
    {
        n = get_float("Change owed: ");
    }
    while (0 > n);
    return n;
}

// converts float number to int multiplied by 100
int dollarsToCents(float dollars)
{
    return round(dollars * 100);
}

// algorithm to calculate required coins
// maybe it's better to abstract those loops or idk, looks terrible
int countCoins(int cents)
{
    int coins = 0;
    while (cents >= 25)
    {
        cents -= 25;
        coins++;
    }
    while (cents >= 10)
    {
        cents -= 10;
        coins++;
    }
    while (cents >= 5)
    {
        cents -= 5;
        coins++;
    }
    return coins += cents;
}

