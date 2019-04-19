#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    //ensure a command line arugment is entered
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    else
    {
        for (int a = 0; a < strlen(argv[1]); a++)
        {
            if (!isalpha(argv[1][a]))
            {
                printf("Usage: ./vigenere k\n");
                return 1;
            }
        }
        string s = get_string("plaintext: ");
        //key setup
        string key = argv[1];
        int m = strlen(key);

        printf("ciphertext: ");
        //use i to keep track of plaintext position
        //use j to keep track of keyword position
        int j = 0;
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            //j is only incremented in the alpha loop
                if (isalpha(s[i]))
                {
                    //maintain lowercase with lower key
                    if (islower(s[i]) && islower(key[j % m]))
                    {
                        printf("%c", (((s[i] - 97) + (key[j % m] - 97)) % 26) + 97);
                    }
                    //maintain lowercase with upper key
                    else if (islower(s[i]) && isupper(key[j % m]))
                    {
                        printf("%c", (((s[i] - 97) + (key[j % m] - 65)) % 26) + 97);
                    }
                    //maintain uppercase with lower key
                    else if (isupper(s[i]) && islower(key[j % m]))
                    {
                        printf("%c", (((s[i] - 65) + (key[j % m] - 97)) % 26) + 65);
                    }
                    //maintain uppercase with upper key
                    else if (isupper(s[i]) && isupper(key[j % m]))
                    {
                        printf("%c", (((s[i] - 65) + (key[j % m] - 65)) % 26) + 65);
                    }
                    j++;
                }
                else //don't alter non-alphabetical characters
                {
                    printf("%c", s[i]);
                }
        }
        printf("\n");
    }
    return 0;
}