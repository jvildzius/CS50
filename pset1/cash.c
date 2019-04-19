#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;
    do
    {
        change = get_float("Change owed: ");
    }
    while (change < 0);
    printf("Change owed: %.2f\n", change);

    //convert dollars to cents (mult by 100 then round off)
    int cents = round(change * 100);
    printf("%i",cents);
    int count = 0;

    //Subtract coins starting with highest value until no more can be subtracted
    while (cents >= 25)
    {
        cents = cents - 25;
        count++;
    }
    while (cents >= 10)
    {
        cents = cents - 10;
        count++;
    }
        while (cents >= 5)
    {
        cents = cents - 5;
        count++;
    }
        while (cents >= 1)
    {
        cents = cents - 1;
        count++;
    }
    //print count of number coins used
    printf ("Count: %i\n",count);

}