#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int shift(char c);

int main(int argc, string argv[])
{
    // argc always == 1 with the name of the file; first parameter is 2
    if (argc == 2)
    {
        // second arg in array though is 1
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            // Check if it's a digit
            if (isdigit(argv[1][i]) != 0)
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
    
    string ptext = get_string("plaintext:\n");
    int n = strlen(ptext);
    string ctext = ptext;
    int keylen = strlen(argv[1]);
    int ct = 0;
    
    for (int i = 0, key = shift(argv[1][0]); i < n; i++)
    {
        // if it's lowercase, go this path
        if (ptext[i] >= 'a' && ptext[i] <= 'z')
        {
            // the key should only apply to letters, don't waste on a space;
            // ct is the count of spaces or other characters that don't need it
            key = shift(argv[1][(i - ct) % keylen]);
            // key can be 0, so start from a, instead of a - 1
            // Then start from 0 when adding
            ctext[i] = ('a') + ((ptext[i] + key - ('a')) % 26);
        }
        else if (ptext[i] <= 'Z' && ptext[i] >= 'A')
        {
            key = shift(argv[1][(i - ct) % keylen]);
            printf("%i, key%i\n", i, key);
            ctext[i] = ('A') + ((ptext[i] + key - ('A')) % 26);
        }
        else
        {
            // increment number of spaces accounted for
            ctext[i] = ptext[i];
            ct++;
        }
    }
    printf("ciphertext:%s\n", ctext);
    return 0;
}

int shift(char c) 
{
    if (c >= 'a' && c <= 'z')
    {
        int key = c - 'a';
        return key;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        int key = c - 'A';
        return key;
    }
    return 0;
}
