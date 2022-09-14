#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    // For each row
    for (int i = 1; i < height + 1; i++)
    {
        // Prints spaces to right-align #s
        for (int j = height - i; j > 0; j--)
        {
            printf(" ");
        }
        // Prints #s after spaces
        for (int n = 0; n < i; n++)
        {
            printf("#");
        }
        //Starts next line
        printf("\n");
    }
}