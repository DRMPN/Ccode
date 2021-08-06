#include "helpers.h"
#include <math.h>

int rgbCut(int a);
int findMin(int a, int min);
int findMax(int a, int max);

// Convert image to grayscale
//
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // calculate avarage of all colors
            int average = round((image[h][w].rgbtBlue + image[h][w].rgbtRed + image[h][w].rgbtGreen) / (float) 3);

            // change all colors to avarege
            image[h][w].rgbtBlue  = average;
            image[h][w].rgbtRed   = average;
            image[h][w].rgbtGreen = average;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++) // for each row of pixels
    {
        for (int w = 0; w < width; w++) // for each pixel
        {
            int originalBlue  = image[h][w].rgbtBlue;  //
            int originalRed   = image[h][w].rgbtRed;   // store original colors
            int originalGreen = image[h][w].rgbtGreen; //

            // change initial pixels to new ones
            image[h][w].rgbtBlue  = rgbCut(round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue));
            image[h][w].rgbtRed   = rgbCut(round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue));
            image[h][w].rgbtGreen = rgbCut(round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue));
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++) // for each row of pixels in an image
    {
        RGBTRIPLE temp[width]; // create temporary row of pixels

        for (int w = 0, i = width - 1; w < width; w++, i--) // store reversed row of pixels of ititial image
        {
            temp[i] = image[h][w];
        }

        for (int w = 0; w < width; w++) // transfer reversed row into initial image
        {
            image[h][w] = temp[w];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // temporary new image to store blured bytes

    for (int h = 0; h < height; h++) // for each pixel row
    {
        for (int w = 0; w < width; w++) // for each pixel
        {
            int avgBlue  = 0;
            int avgRed   = 0;
            int avgGreen = 0;
            float count  = 0;

            for (int i = findMin(h, 0); i <= findMax(h, height - 1); i++) // for each pixel row that is in range of initial pixel
            {
                for (int n = findMin(w, 0); n <= findMax(w, width - 1); n++) // for each pixel that is in range of initial pixel
                {
                    avgBlue  += image[i][n].rgbtBlue;   //
                    avgRed   += image[i][n].rgbtRed;    // store sum of colors
                    avgGreen += image[i][n].rgbtGreen;  //
                    count++;
                }
            }

            temp[h][w].rgbtBlue  = round(avgBlue / count);  //
            temp[h][w].rgbtRed   = round(avgRed / count);   // calculate average amount of colors
            temp[h][w].rgbtGreen = round(avgGreen / count); //
        }
    }

    for (int h = 0; h < height; h++) // transfer temp bytes to initial image to change it
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtBlue  = temp[h][w].rgbtBlue;
            image[h][w].rgbtRed   = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
        }
    }
}

// if input value is more than 255, return 255, else return same value
int rgbCut(int a)
{
    if (a > 255)
    {
        return 255;
    }
    else
    {
        return a;
    }
}

// helpers to determine eligible range for box collision
// range is 1, can abstract
int findMin(int a, int min)
{
    if (a - 1 < min)
    {
        return a;
    }
    else
    {
        return a - 1;
    }
}
int findMax(int a, int max)
{
    if (a + 1 > max)
    {
        return a;
    }
    else
    {
        return a + 1;
    }
}

/*
first variant of grayscale

int max = maxOfThree(image[h][w].rgbtBlue, image[h][w].rgbtRed, image[h][w].rgbtGreen);

            image[h][w].rgbtBlue = max;
            image[h][w].rgbtRed = max;
            image[h][w].rgbtGreen = max;

int maxOfThree(int a, int b, int c)
{
    if (a >= b && a >= c)
    {
        return a;
    }
    else if (b >= a && b >= c)
    {
        return b;
    }
    else
    {
        return c;
    }
}

*/