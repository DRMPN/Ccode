#include <stdio.h>
#include <cs50.h>

// CONSTANTS:
char emptyBlock = ' ';
char brickBlock = '#';
int minHeight = 1;
int maxHeight = 8;
string distanceBetweenPyramids = "  ";

// FUNCITONS:
int getAppropriateHeight(void);

int main(void)
{
    int height = getAppropriateHeight();

    for (int line = 1; line <= height; line++)
    {
        // Prints left side of the pyramid
        // print emptyBlock for one line
        for (int emptySpace = height; emptySpace > line; emptySpace--)
        {
            printf("%c", emptyBlock);
        }
        // print brickBlock for one line
        for (int filledSpace = 0; filledSpace < line; filledSpace++)
        {
            printf("%c", brickBlock);
        }

        // Prints right side of the pyramid
        printf("%s", distanceBetweenPyramids);
        for (int filledSpace = line; filledSpace > 0; filledSpace --)
        {
            printf("%c", brickBlock);
        }
        printf("\n");
    }
}

// get appropriate height value from user; must be minHeight < x < maxHeight
int getAppropriateHeight(void)
{
    int n;

    do
    {
        n = get_int("Height: ");
    }
    while (n < minHeight || n > maxHeight);

    return n;
}