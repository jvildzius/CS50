#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Prompt user for appropriate height
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h > 23 || h < 0);

    for (int i = 0; i < h; i++)
    {
        for (int s = 0; s<h-i-1; s++)
        //print spaces h-i-1
        {
            printf(" ");
        }
        //print hashes = i + 2
        for (int x=0; x<i+2;x++)
        {
            printf("#");
        }
        //print new line
        printf("\n");
    }

}