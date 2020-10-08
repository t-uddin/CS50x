#include <stdio.h>
#include <cs50.h>

int main(void)
{
   //prompt user for height until positive integer between 1 and 8
   int height;
   do
   {
       height = get_int("Height: ");

   }
    while (height < 1 || height > 8);

    //print triangle
    for (int r = 0; r < height; r++)
    {
        //print spaces before triangle
        for(int sp = 0; sp < height - r - 1; sp++)
        {
            printf(" ");
        }
        //print hashes of triangle
        for (int c = 0; c <= r; c++)
        {
            printf("#");
        }
        //new line before restarting loop
        printf("\n");
    }

}