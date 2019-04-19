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
        printf("Please enter a key and plaintext to be coded\n");
        return 1;
    }
    else
    {
        string s = get_string("plaintext: ");
        //changing the users input for the key to an int
        string num = argv[1];
        int key = atoi(num);

        printf("ciphertext: ");
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if (isalpha(s[i]))
            {
                //maintain lowercase
                if (islower(s[i]))
                {
                    //shift character by the key
                    printf("%c", (((s[i] + key) - 97) % 26) + 97);
                }
                //maintain uppercase
                else if (isupper(s[i]))
                {
                    //shit character by the key
                    printf("%c", (((s[i] + key) - 65) % 26) + 65);
                }
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