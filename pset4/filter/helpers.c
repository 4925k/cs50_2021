#include "helpers.h"
#include <stdio.h>
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //take average of all colors and assign it to all colors
            float blue = image[h][w].rgbtBlue;
            float green = image[h][w].rgbtGreen;
            float red = image[h][w].rgbtRed;
            int avg = round((red + blue + green) / 3);
            image[h][w].rgbtBlue = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            //simple swapping algorithm to swap pixels
            RGBTRIPLE temp = image[h][w];
            //minus 1 to get last pixel at the first
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make another image to store pixel values
    //so that original values do not get lost for calculations
    RGBTRIPLE tempImage[height][width];

    //loop over all pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int sumBlue = 0;
            int sumGreen = 0;
            int sumRed = 0;
            //count to store how many pixels were counted
            float count = 0;

            //loop for box around pixel
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    //if the pixel is not out of bounds
                    if (h + r < 0 || h + r >= height || w + c < 0 || w + c >= width)
                    {
                        continue;
                    }

                    //getting sum step by step
                    sumBlue += image[h + r][w + c].rgbtBlue;
                    sumGreen += image[h + r][w + c].rgbtGreen;
                    sumRed += image[h + r][w + c].rgbtRed;
                    count++;

                }
            }

            //keep avg values to blur
            tempImage[h][w].rgbtBlue = round(sumBlue / count);
            tempImage[h][w].rgbtGreen = round(sumGreen / count);
            tempImage[h][w].rgbtRed = round(sumRed / count);

        }
    }


    //at last copy the new pixels values to the actual image.
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImage[h][w];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temp image to store new values
    //and not lose old values
    RGBTRIPLE tempImage[height][width];

    //sobel operator vairables
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //loop over each pixel
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //inititalize variables to store sums for Gx and Gy
            float gxRed, gxBlue, gxGreen, gyRed, gyBlue, gyGreen;
            int gFinalRed, gFinalBlue, gFinalGreen;
            gxRed = gxBlue = gxGreen = gyRed = gyBlue = gyGreen = 0;
            gFinalRed = gFinalBlue = gFinalGreen = 0;


            //loop for box around the pixel
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    //check for edges
                    if (h + r < 0 || h + r >= height || w + c < 0 || w + c >= width)
                    {
                        continue;

                    }
                    gxRed += gx[r + 1][c + 1] * image[h + r][w + c].rgbtRed;
                    gxBlue += gx[r + 1][c + 1] * image[h + r][w + c].rgbtBlue;
                    gxGreen += gx[r + 1][c + 1] * image[h + r][w + c].rgbtGreen;
                    gyRed += gy[r + 1][c + 1] * image[h + r][w + c].rgbtRed;
                    gyBlue += gy[r + 1][c + 1] * image[h + r][w + c].rgbtBlue;
                    gyGreen += gy[r + 1][c + 1] * image[h + r][w + c].rgbtGreen;
                }
            }


            //use sobel algorithm to get a single value
            gFinalRed = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            if (gFinalRed > 255)
            {
                gFinalRed = 255;
            }
            gFinalBlue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            if (gFinalBlue > 255)
            {
                gFinalBlue = 255;
            }
            gFinalGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            if (gFinalGreen > 255)
            {
                gFinalGreen = 255;
            }

            tempImage[h][w].rgbtBlue = gFinalBlue;
            tempImage[h][w].rgbtGreen = gFinalGreen;
            tempImage[h][w].rgbtRed = gFinalRed;
        }
    }
    //copy temp image to actual file
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = tempImage[h][w];
        }
    }
    return;
}
