#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
  //Prompt user for change amount
  float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change <= 0);

    //Convert dollars to cents and convert to integer to prevent floating point imprecision
    int cents = round(change * 100);

    //Print minimum number of coins to return
    int coins = 0;

    for (int quarters = 0; cents >= 25; quarters++)
    {
        cents -= 25;
        coins++;
    }

    for (int dimes = 0; cents >=10; dimes++)
    {
        cents -= 10;
        coins++;
    }

    for (int nickels = 0; cents>=5; nickels++)
    {
        cents -= 5;
        coins++;
    }

    for (int dimes = 0; cents>=1; dimes++)
    {
        cents -= 1;
        coins++;
    }

    printf("%i\n", coins);
}