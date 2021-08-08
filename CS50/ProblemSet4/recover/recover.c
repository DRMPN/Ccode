#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Constant
#define BLOCK_SIZE 512

typedef uint8_t BYTE;

// Function prototype
int find_jpeg(BYTE b[]);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover filename.\n");
        return 1;
    }

    // Open input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open a file.\n");
        return 1;
    }

    // counter for images
    int count = 0;

    // allocate memory for a filename string, needed for sprintf
    char *filename = malloc(8);
    FILE *output;

    // create buffer
    BYTE buffer[BLOCK_SIZE];
    while (fread(&buffer, sizeof(BYTE), BLOCK_SIZE, input))
    {
        if (find_jpeg(buffer))
        {
            // if first image was found
            if (count != 0)
            {
                // close previous file
                fclose(output);
            }

            // generate name for a file
            sprintf(filename, "%03i.jpg", count);
            count++;

            // create new file to write data
            output = fopen(filename, "w");
            if (output == NULL)
            {
                printf("Could not create a file.\n");
                // probably needed fclose(input);
                return 1;
            }

            // write first block
            fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
        else
        {
            // if found first jpeg
            if (count != 0)
            {
                // continue writing blocks
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, output);
            }
            // else skip cuz have not found any jpeg yet
        }
    }

    // free memory for filename
    free(filename);

    // close output file
    fclose(output);

    // close input file
    fclose(input);
}

// checkes wheather or not first 4 bytes of a block are jpeg bytes
int find_jpeg(BYTE b[])
{
    if (b[0] == 0xff && b[1] == 0xd8 && b[2] == 0xff && (b[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}