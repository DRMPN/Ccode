#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// special struct to store calculated Gx and Gy integers
typedef struct
{
    int rgbtBlue;
    int rgbtGreen;
    int rgbtRed;
}
INT_TRIPLE;

int rgbCut(int a);
int findMin(int a, int min);
int findMax(int a, int max);
void empty_triple(INT_TRIPLE *t);

// Convert image to grayscale
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
// TODO rewrite it with INT_TRIPLE usage, was lazy to spend 10 more minutes to do so :P
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

            for (int i = h - 1; i <= h + 1; i++) // for each pixel row that is in range of initial pixel
            {
                for (int n = w - 1; n <= w + 1; n++) // for each pixel that is in range of initial pixel
                {
                    if (i < 0 || i > height - 1 || n < 0 || n > width - 1)
                    {
                        // do nothing
                    }
                    else
                    {
                        avgBlue  += image[i][n].rgbtBlue;   //
                        avgRed   += image[i][n].rgbtRed;    // store sum of colors
                        avgGreen += image[i][n].rgbtGreen;  //
                        count++;
                    }
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // best to make it heap and allocate memory and also check for not null and etc but who cares xD
    // probably just copypaste from filter.c *image declaration
    RGBTRIPLE temp[height][width]; // temporary new image to store computed bytes

    // sobel's kernels
    int Gx_kernel[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy_kernel[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // allocating free memory for operation
    INT_TRIPLE *Gx_average = malloc(sizeof(INT_TRIPLE));
    INT_TRIPLE *Gy_average = malloc(sizeof(INT_TRIPLE));

    if (Gx_average == NULL || Gy_average == NULL)
    {
        printf("Not enough memory to process pixels through Sobel filter.\n Further error message unavoidable.\n");
        // something should be here but this function doesn't return a value, and I don't know a way to stop calculations
        // probably have to rewrite code so this will return a value or just don't use mem alloc here?
    }

    for (int h = 0; h < height; h++) // for each pixel row
    {
        for (int w = 0; w < width; w++) // for each pixel
        {
            int count = 0; // didn't want to wrap another loop over it, my "elegant" solution

            // clear triples for further calculations
            empty_triple(Gx_average);
            empty_triple(Gy_average);

            for (int i = h - 1; i <= h + 1; i++) // for each pixel row that is in range of initial pixel
            {
                for (int n = w - 1; n <= w + 1; n++) // for each pixel that is in range of initial pixel
                {
                    if (i < 0 || i > height - 1 || n < 0 || n > width - 1) // out of bound case
                    {
                        // don't need to calculate it for black pixel
                    }
                    else
                    {
                        // maybe there's a way to abstract it?
                        // compute gx for normal pixel
                        (*Gx_average).rgbtBlue  += Gx_kernel[count] * image[i][n].rgbtBlue;
                        (*Gx_average).rgbtGreen += Gx_kernel[count] * image[i][n].rgbtGreen;
                        (*Gx_average).rgbtRed   += Gx_kernel[count] * image[i][n].rgbtRed;

                        // compute gy for normal pixel
                        (*Gy_average).rgbtBlue  += Gy_kernel[count] * image[i][n].rgbtBlue;
                        (*Gy_average).rgbtGreen += Gy_kernel[count] * image[i][n].rgbtGreen;
                        (*Gy_average).rgbtRed   += Gy_kernel[count] * image[i][n].rgbtRed;
                    }
                    count++;
                }
            }

            //square root of (gx^2 + gy^2) -> round -> cut it if more than 255 -> store in temp
            temp[h][w].rgbtBlue = rgbCut(round(sqrt(pow((*Gx_average).rgbtBlue, 2) + pow((*Gy_average).rgbtBlue, 2))));
            temp[h][w].rgbtGreen = rgbCut(round(sqrt(pow((*Gx_average).rgbtGreen, 2) + pow((*Gy_average).rgbtGreen, 2))));
            temp[h][w].rgbtRed = rgbCut(round(sqrt(pow((*Gx_average).rgbtRed, 2) + pow((*Gy_average).rgbtRed, 2))));
        }
    }

    // free allocated memory
    free(Gx_average);
    free(Gy_average);

    // transfer temp bytes to initial image to change it
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w].rgbtBlue  = temp[h][w].rgbtBlue;
            image[h][w].rgbtRed   = temp[h][w].rgbtRed;
            image[h][w].rgbtGreen = temp[h][w].rgbtGreen;
        }
    }
}

// pretty straight forward
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

// pretty straight forward too, changes values of a triple at 0x memory location to zero
void empty_triple(INT_TRIPLE *t)
{
    (*t).rgbtBlue = 0;
    (*t).rgbtGreen = 0;
    (*t).rgbtRed = 0;
}