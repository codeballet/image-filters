#include "helpers.h"
#include <math.h>
#include <stdbool.h>

// Prototypes
RGBTRIPLE blurred_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width]);
int channel_check(int sValue);
RGBTRIPLE edged_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width]);
int calc_pixel(int gx, int gy);
bool valid_pixel(int i, int j, int height, int width);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average from color channels for a pixel
            int average = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);

            // Set all color channels to average
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get original color channel values
            int oBlue = image[i][j].rgbtBlue;
            int oGreen = image[i][j].rgbtGreen;
            int oRed = image[i][j].rgbtRed;

            // Apply sepia algorithm for each channel
            int sBlue = round(.272 * oRed + .534 * oGreen + .131 * oBlue);
            int sGreen = round(.349 * oRed + .686 * oGreen + .168 * oBlue);
            int sRed = round(.393 * oRed + .769 * oGreen + .189 * oBlue);

            // Check and correct if any channel exceeds 255
            image[i][j].rgbtBlue = channel_check(sBlue);
            image[i][j].rgbtGreen = channel_check(sGreen);
            image[i][j].rgbtRed = channel_check(sRed);
        }
    }
    return;
}

// Check and correct any channel exceeding 255
int channel_check(int sValue)
{
    if (sValue > 255)
    {
        return 255;
    }
    else
    {
        return sValue;
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Set a swap value for counting width
        int swap = width - 1;

        for (int j = 0; j < width / 2; j++)
        {
            // Create temporary channel values for swapping
            int tBlue = image[i][swap].rgbtBlue;
            int tGreen = image[i][swap].rgbtGreen;
            int tRed = image[i][swap].rgbtRed;

            // Move left pixels to right
            image[i][swap].rgbtBlue = image[i][j].rgbtBlue;
            image[i][swap].rgbtGreen = image[i][j].rgbtGreen;
            image[i][swap].rgbtRed = image[i][j].rgbtRed;

            // Assign the temporary value to left pixels
            image[i][j].rgbtBlue = tBlue;
            image[i][j].rgbtGreen = tGreen;
            image[i][j].rgbtRed = tRed;

            swap--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an array to store the blurred image
    RGBTRIPLE blurred_image[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blurred_image[i][j] = blurred_pixel(i, j, height, width, image);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred_image[i][j];
        }
    }
    return;
}

// Generate a blurred pixel
RGBTRIPLE blurred_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int blues = 0;
    int greens = 0;
    int reds = 0;
    int pixels = 0;
    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            int calc_i = i + di;
            int calc_j = j + dj;

            // if valid pixel, add up the color channels per color
            if (valid_pixel(calc_i, calc_j, height, width))
            {
                pixels++;
                blues += image[calc_i][calc_j].rgbtBlue;
                greens += image[calc_i][calc_j].rgbtGreen;
                reds += image[calc_i][calc_j].rgbtRed;
            }
        }
    }

    // Calculate the average color channel values
    RGBTRIPLE av_blurred_pixel;
    av_blurred_pixel.rgbtBlue = round((float) blues / pixels);
    av_blurred_pixel.rgbtGreen = round((float) greens / pixels);
    av_blurred_pixel.rgbtRed = round((float) reds / pixels);

    return av_blurred_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create an array to hold the edged image
    RGBTRIPLE edged_image[height][width];

    // Generate an edged image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edged_image[i][j] = edged_pixel(i, j, height, width, image);
        }
    }

    // Copy the edged array to original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged_image[i][j];
        }
    }
    return;
}

// Generate an edged pixel
RGBTRIPLE edged_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int bluesGx = 0;
    int bluesGy = 0;
    int greensGx = 0;
    int greensGy = 0;
    int redsGx = 0;
    int redsGy = 0;

    // Define the Sobel kernel matrix values
    int arr_Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int arr_Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int arr_count = 0;

    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            int calc_i = i + di;
            int calc_j = j + dj;

            // if valid pixel, add up the Gx Gy color channel values per color
            if (valid_pixel(calc_i, calc_j, height, width))
            {
                bluesGx += image[calc_i][calc_j].rgbtBlue * arr_Gx[arr_count];
                bluesGy += image[calc_i][calc_j].rgbtBlue * arr_Gy[arr_count];
                greensGx += image[calc_i][calc_j].rgbtGreen * arr_Gx[arr_count];
                greensGy += image[calc_i][calc_j].rgbtGreen * arr_Gy[arr_count];
                redsGx += image[calc_i][calc_j].rgbtRed * arr_Gx[arr_count];
                redsGy += image[calc_i][calc_j].rgbtRed * arr_Gy[arr_count];
            }

            arr_count++;
        }
    }

    RGBTRIPLE final_pixel;
    final_pixel.rgbtBlue = calc_pixel(bluesGx, bluesGy);
    final_pixel.rgbtGreen = calc_pixel(greensGx, greensGy);
    final_pixel.rgbtRed = calc_pixel(redsGx, redsGy);

    return final_pixel;
}

int calc_pixel(int gx, int gy)
{
    int result = round(sqrt(gx * gx + gy * gy));

    return result < 255 ? result : 255;
}

bool valid_pixel(int i, int j, int height, int width)
{
    return i >= 0 && i < height && j >= 0 && j < width;
}