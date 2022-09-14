#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define BYTE structure
typedef uint8_t BYTE;

// Global variable
#define BLOCK_SIZE 512


int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open input file
    FILE *input_file = fopen(argv[1], "r");

    if (input_file == NULL)
    {
        printf("Error: Could not open\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];

    // Number of images counted
    int imgCount = 0;

    // Where (pointer) the file will be written to
    FILE *image_pointer = NULL;

    // Stores filename (8 bytes for the name, including the \0 nul)
    char filename[8];

    // Loops until end of input_file
    while (fread(buffer, 1, BLOCK_SIZE, input_file) == BLOCK_SIZE)
    {
        // Checks first 4 bytes to see if beginning of .jpg
        if (buffer[0] ==  0xff && buffer[1] ==  0xd8 && buffer[2] ==  0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If there is an image, close the file
            if (imgCount != 0)
            {
                fclose(image_pointer);
            }
            // Give a new .jpg file name in the array filename
            sprintf(filename, "%03i.jpg", imgCount);

            // File image_pointer points to the a new space called filename
            image_pointer = fopen(filename, "w");

            // Adds an image
            imgCount++;

        }
        // Writes data to the array "buffer" from the data from the file pointer image_pointer
        if (imgCount != 0)
        {
            fwrite(buffer, 512, 1, image_pointer);
        }

    }
    fclose(input_file);
    fclose(image_pointer);

    return 0;

}