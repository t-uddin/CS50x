#include <stdio.h>
#include <cs50.h>

int main(void)
{
   int height;
   do
   {
       height = get_int("Height: ");

   }
    while (height < 1 || height > 8);

    for (int r = 0; r < height; r++)
    {

        for(int sp = 0; sp < height - r - 1; sp++)
        {
            printf(" ");
        }
        for (int c = 0; c <= r; c++)
        {
            printf("#");
        }

        printf("\n");
    }

}