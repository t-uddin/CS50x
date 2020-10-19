#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Iterate over every row
    for (int i = 0; i < height; i++)
    {
        //iterate over every pixel in row
        for (int j = 0; j < width; j++)
        {
            //Average the colour values of that pixel and round to int
            float avg = round((image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
       //Iterate over every row
    for (int i = 0; i < height; i++)
    {
        //iterate over every pixel in row
        for (int j = 0; j < width; j++)
        {
            //Calculate each colour in pixel to sepia colour, rounding to intand cap at 255

            int sepiaRed = round(.393 *  image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

        //Apply new sepia RGB values to RGB values, capping at 255
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;

}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Iterate over each row
     for (int i = 0; i < height; i++)
     {
         //Iterate over ever pixel and change it's adress from left to right side of image
         for (int j = 0; j < width / 2; j++)
         {
           RGBTRIPLE temp = image[i][j];
           image[i][j] = image[i][width - j - 1];
           image[i][width - j - 1] = temp;
        }
     }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float sumRed = 0;
    float sumGreen = 0;
    float sumBlue = 0;
    float counter = 0;

    //Create temp container for image so no changes to original image that would affect averages
    RGBTRIPLE temp[height][width];

    //Iterate over each row
    for (int i = 0; i < height; i++)
    {
        //Iterate over each pixel within row
        for (int j = 0; j < height; j++)
        {
            //Sum r, g and b values for pixels within 1 row/column of pixel[i][j]
            for (int k = -1; k < 1; k++)
            {
                for (int l = -1; l <1; l++)
                {
                    //Only sum if adress is of a pixel within picture for edges/corners
                    if(i + l > -1 && j + k > -1)
                    {
                        sumRed = sumRed + image[i + l][j + k].rgbtRed;
                        sumBlue = sumBlue + image[i + l][j + k].rgbtBlue;
                        sumGreen = sumGreen + image[i + l][j + k].rgbtRed;

                        counter++;
                    }
                }
            }

        //Apply average colours to that pixel in temp picture
        temp[i][j].rgbtRed = round(sumRed / counter);
        temp[i][j].rgbtGreen = round(sumGreen / counter);
        temp[i][j].rgbtBlue = round(sumBlue / counter);

        }
    }

    //Now copy temp image pixel colours into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }

    }

    return;
}
