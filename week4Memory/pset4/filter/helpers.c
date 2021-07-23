#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculate the average
            BYTE average = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // width - j - 1 is the position which i should be after reflect
            RGBTRIPLE temp = image[i][width - j - 1];
            image[i][width - j - 1] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // use a new map to avoid the problem raised after the image[i][j] value changed
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //cant use int because when doing the division, int will contract the value and make the round() result wrong
            float Blue = 0;
            float Green = 0;
            float Red = 0;
            int counter = 0;
            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    // mean over the edge, dont add this position value
                    if (i + m < 0 || i + m > height - 1)
                    {
                        continue;
                    }
                    // mean over the edge, dont add this position value
                    if (j + n < 0 || j + n > width - 1)
                    {
                        continue;
                    }
                    Blue += temp[i + m][j + n].rgbtBlue;
                    Green += temp[i + m][j + n].rgbtGreen;
                    Red += temp[i + m][j + n].rgbtRed;
                    counter++;
                }
            }
            image[i][j].rgbtBlue = round(Blue / counter);
            image[i][j].rgbtGreen = round(Green / counter);
            image[i][j].rgbtRed = round(Red / counter);
        }

    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // use a new map to avoid the problem raised after the image[i][j] value changed
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // the kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxBlue = 0;
            int gxGreen = 0;
            int gxRed = 0;

            int gyBlue = 0;
            int gyGreen = 0;
            int gyRed = 0;

            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if (i + m < 0 || i + m > height - 1)
                    {
                        continue;
                    }
                    if (j + n < 0 || j + n > width - 1)
                    {
                        continue;
                    }
                    gxBlue += temp[i + m][j + n].rgbtBlue * Gx[m + 1][n + 1];
                    gxGreen += temp[i + m][j + n].rgbtGreen * Gx[m + 1][n + 1];
                    gxRed += temp[i + m][j + n].rgbtRed * Gx[m + 1][n + 1];

                    gyBlue += temp[i + m][j + n].rgbtBlue * Gy[m + 1][n + 1];
                    gyGreen += temp[i + m][j + n].rgbtGreen * Gy[m + 1][n + 1];
                    gyRed += temp[i + m][j + n].rgbtRed * Gy[m + 1][n + 1];
                }
            }
            int Blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            int Green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int Red = round(sqrt(gxRed * gxRed + gyRed * gyRed));

            if (Blue > 255)
            {
                Blue = 255;
            }
            if (Green > 255)
            {
                Green = 255;
            }
            if (Red > 255)
            {
                Red = 255;
            }
            image[i][j].rgbtBlue = Blue;
            image[i][j].rgbtGreen = Green;
            image[i][j].rgbtRed = Red;
        }
    }
    return;
}
