#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through every pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Averages the colors
            int avg = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through every pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Calculates sepia colors
            int sepiaRed = round(.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            int sepiaGreen = round(.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            int sepiaBlue = round(.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            // Adds sepia values to respective colors
            image[h][w].rgbtRed = sepiaRed;
            image[h][w].rgbtGreen = sepiaGreen;
            image[h][w].rgbtBlue = sepiaBlue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tempR, tempG, tempB;
    // Loop through every pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            // Moves opposite pixel to a temp variable
            tempR = image[h][width - w - 1].rgbtRed;
            tempG = image[h][width - w - 1].rgbtGreen;
            tempB = image[h][width - w - 1].rgbtBlue;
            // Opposite pixel becomes current pixel
            image[h][width - w - 1].rgbtRed = image[h][w].rgbtRed;
            image[h][width - w - 1].rgbtGreen = image[h][w].rgbtGreen;
            image[h][width - w - 1].rgbtBlue = image[h][w].rgbtBlue;
            //Current pixel becomes temp (opposite) pixel
            image[h][w].rgbtRed = tempR;
            image[h][w].rgbtGreen = tempG;
            image[h][w].rgbtBlue = tempB;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize copy of image
    RGBTRIPLE copy[height][width];
    int avgR, avgG, avgB, count;

    // Adds image into the copy
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            copy[h][w].rgbtRed = image[h][w].rgbtRed;
            copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
        }
    }

    // Loop to blur the pixels using averages
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //If in the center of the image
            if (h != 0 && h != height - 1 && w != 0 && w != width - 1)
            {
                avgR = round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                              copy[h][w - 1].rgbtRed + copy[h][w].rgbtRed + copy[h][w + 1].rgbtRed +
                              copy[h + 1][w - 1].rgbtRed + copy[h + 1][w].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 9.0);
                avgG = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                              copy[h][w - 1].rgbtGreen + copy[h][w].rgbtGreen + copy[h][w + 1].rgbtGreen +
                              copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 9.0);
                avgB = round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                              copy[h][w - 1].rgbtBlue + copy[h][w].rgbtBlue + copy[h][w + 1].rgbtBlue +
                              copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 9.0);
            }
            // If in the first row
            else if (h == 0)
            {
                if (w == 0)
                {
                    avgR = round((copy[h][w].rgbtRed + copy[h][w + 1].rgbtRed + copy[h + 1][w].rgbtRed +
                                  copy[h + 1][w + 1].rgbtRed) / 4.0);
                    avgG = round((copy[h][w].rgbtGreen + copy[h][w + 1].rgbtGreen + copy[h + 1][w].rgbtGreen +
                                  copy[h + 1][w + 1].rgbtGreen) / 4.0);
                    avgB = round((copy[h][w].rgbtBlue + copy[h][w + 1].rgbtBlue + copy[h + 1][w].rgbtBlue +
                                  copy[h + 1][w + 1].rgbtBlue) / 4.0);
                }
                else if (w == width - 1)
                {
                    avgR = round((copy[h][w - 1].rgbtRed + copy[h][w].rgbtRed + copy[h + 1][w - 1].rgbtRed +
                                  copy[h + 1][w].rgbtRed) / 4.0);
                    avgG = round((copy[h][w - 1].rgbtGreen + copy[h][w].rgbtGreen + copy[h + 1][w - 1].rgbtGreen +
                                  copy[h + 1][w].rgbtGreen) / 4.0);
                    avgB = round((copy[h][w - 1].rgbtBlue + copy[h][w].rgbtBlue + copy[h + 1][w - 1].rgbtBlue +
                                  copy[h + 1][w].rgbtBlue) / 4.0);
                }
                else
                {
                    avgR = round((copy[h][w - 1].rgbtRed + copy[h][w].rgbtRed + copy[h][w + 1].rgbtRed +
                                  copy[h + 1][w - 1].rgbtRed + copy[h + 1][w].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 6.0);
                    avgG = round((copy[h][w - 1].rgbtGreen + copy[h][w].rgbtGreen + copy[h][w + 1].rgbtGreen +
                                  copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 6.0);
                    avgB = round((copy[h][w - 1].rgbtBlue + copy[h][w].rgbtBlue + copy[h][w + 1].rgbtBlue +
                                  copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 6.0);
                }
            }
            // If in the last row
            else if (h == height - 1)
            {
                if (w == 0)
                {
                    avgR = round((copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed + copy[h][w].rgbtRed +
                                  copy[h][w + 1].rgbtRed) / 4.0);
                    avgG = round((copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen + copy[h][w].rgbtGreen +
                                  copy[h][w + 1].rgbtGreen) / 4.0);
                    avgB = round((copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue + copy[h][w].rgbtBlue +
                                  copy[h][w + 1].rgbtBlue) / 4.0);
                }
                else if (w == width - 1)
                {
                    avgR = round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed + copy[h][w - 1].rgbtRed +
                                  copy[h][w].rgbtRed) / 4.0);
                    avgG = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h][w - 1].rgbtGreen +
                                  copy[h][w].rgbtGreen) / 4.0);
                    avgB = round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h][w - 1].rgbtBlue +
                                  copy[h][w].rgbtBlue) / 4.0);
                }
                else
                {
                    avgR = round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed +
                                  copy[h][w - 1].rgbtRed + copy[h][w].rgbtRed + copy[h][w + 1].rgbtRed) / 6.0);
                    avgG = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen +
                                  copy[h][w - 1].rgbtGreen + copy[h][w].rgbtGreen + copy[h][w + 1].rgbtGreen) / 6.0);
                    avgB = round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue +
                                  copy[h][w - 1].rgbtBlue + copy[h][w].rgbtBlue + copy[h][w + 1].rgbtBlue) / 6.0);
                }
            }
            // If on the left edge
            else if (w == 0)
            {
                avgR = round((copy[h - 1][w].rgbtRed + copy[h - 1][w + 1].rgbtRed + copy[h][w].rgbtRed +
                              copy[h][w + 1].rgbtRed + copy[h + 1][w].rgbtRed + copy[h + 1][w + 1].rgbtRed) / 6.0);
                avgG = round((copy[h - 1][w].rgbtGreen + copy[h - 1][w + 1].rgbtGreen + copy[h][w].rgbtGreen +
                              copy[h][w + 1].rgbtGreen + copy[h + 1][w].rgbtGreen + copy[h + 1][w + 1].rgbtGreen) / 6.0);
                avgB = round((copy[h - 1][w].rgbtBlue + copy[h - 1][w + 1].rgbtBlue + copy[h][w].rgbtBlue +
                              copy[h][w + 1].rgbtBlue + copy[h + 1][w].rgbtBlue + copy[h + 1][w + 1].rgbtBlue) / 6.0);
            }
            // If on the right edge
            else if (w == width - 1)
            {
                avgR = round((copy[h - 1][w - 1].rgbtRed + copy[h - 1][w].rgbtRed + copy[h][w - 1].rgbtRed +
                              copy[h][w].rgbtRed + copy[h + 1][w - 1].rgbtRed + copy[h + 1][w].rgbtRed) / 6.0);
                avgG = round((copy[h - 1][w - 1].rgbtGreen + copy[h - 1][w].rgbtGreen + copy[h][w - 1].rgbtGreen +
                              copy[h][w].rgbtGreen + copy[h + 1][w - 1].rgbtGreen + copy[h + 1][w].rgbtGreen) / 6.0);
                avgB = round((copy[h - 1][w - 1].rgbtBlue + copy[h - 1][w].rgbtBlue + copy[h][w - 1].rgbtBlue +
                              copy[h][w].rgbtBlue + copy[h + 1][w - 1].rgbtBlue + copy[h + 1][w].rgbtBlue) / 6.0);
            }
            // Adds the new blurred pixel
            image[h][w].rgbtRed = avgR;
            image[h][w].rgbtGreen = avgG;
            image[h][w].rgbtBlue = avgB;
        }
    }

    return;
}
