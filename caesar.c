#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // argc always == 1 with the name of the file; first parameter is 2
    if (argc == 2)
    {
        // second arg in array though is 1
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            // Check if it's a valid digit
            if (isdigit(argv[1][i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }
    else 
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    int key = atoi(argv[1]);
    string ptext = get_string("plaintext:\n");
    int n = strlen(ptext);
    string ctext = ptext;
    
    for (int i = 0; i < n; i++)
    {
        // if it's lowercase, go this path
        if (ptext[i] >= 'a' && ptext[i] <= 'z')
        {
            // Need to use a - 1 as the base, since we're starting from 0 in the mod
            // Then start from 0 when adding
            ctext[i] = ('a' - 1) + ((ptext[i] + key - ('a' - 1)) % 26);
        }
        else if (ptext[i] <= 'Z' && ptext[i] >= 'A')
        {
            ctext[i] = ('A' - 1) + ((ptext[i] + key - ('A' - 1)) % 26);
        }
    }
    printf("ciphertext:%s\n", ctext);
    return 0;
}
