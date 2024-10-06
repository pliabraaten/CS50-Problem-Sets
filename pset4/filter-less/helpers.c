#include <math.h>

#include "helpers.h"

// Prototypes
int rgb_value_check(int sepia_value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each row of pixels
    for (int i = 0; i < height; i++)
    {
        // Loop through each pixel in row
        for (int j = 0; j < width; j++)
        {
            // Calculate average values of rbg
            int avg_rgb =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Update pixel rbg values to the average
            image[i][j].rgbtBlue = avg_rgb;
            image[i][j].rgbtGreen = avg_rgb;
            image[i][j].rgbtRed = avg_rgb;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize sepia valued variables
    int sepia_red = 0;
    int sepia_green = 0;
    int sepia_blue = 0;

    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Use sepia formula to calculate new sepia rgb values based on original rgb values
            // RED
            sepia_red = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                              0.189 * image[i][j].rgbtBlue);

            // GREEN
            sepia_green = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                                0.168 * image[i][j].rgbtBlue);

            // BLUE
            sepia_blue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                               0.131 * image[i][j].rgbtBlue);

            // Update new sepia values (function limits value between 0 and 255)
            image[i][j].rgbtRed = rgb_value_check(sepia_red);
            image[i][j].rgbtGreen = rgb_value_check(sepia_green);
            image[i][j].rgbtBlue = rgb_value_check(sepia_blue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Temp storage place for pixel swap
    RGBTRIPLE temp;
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixels
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image to maintain original pixel values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Create variable for average calculation
    RGBTRIPLE average;

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average rgb value (average pixel values of 9 adjacent pixels)
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;
            int average_count = 0;

            // Loop through adjacent pixels in 3x3 grid with dx and dy to find relative position to
            // the inputted pixel
            for (int dx = -1; dx < 2; dx++)
            {
                for (int dy = -1; dy < 2; dy++)
                {
                    // Check if adjacent pixel is within height bounds of the image
                    if (dx + i >= 0 && dx + i < height)
                    {
                        // Check if adjacent pixel is within width bounds of the image
                        if (dy + j >= 0 && dy + j < width)
                        {
                            // Add adjacent pixel's rgb value to running total
                            total_red += copy[dx + i][dy + j].rgbtRed;
                            total_green += copy[dx + i][dy + j].rgbtGreen;
                            total_blue += copy[dx + i][dy + j].rgbtBlue;
                            average_count++;
                        }
                    }
                }
            }
            // Calculate rgb value averages
            average.rgbtRed = round((float) total_red / average_count);
            average.rgbtGreen = round((float) total_green / average_count);
            average.rgbtBlue = round((float) total_blue / average_count);

            // Update image with average pixel value
            image[i][j] = average;
        }
    }
    return;
}

// Check if adjusted pixel rgb values are between 0 and 255
int rgb_value_check(int sepia_value)
{
    if (sepia_value > 255)
    {
        return 255;
    }
    else if (sepia_value < 0)
    {
        return 0;
    }
    else
    {
        return sepia_value;
    }
}
