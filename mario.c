#include <cs50.h>
#include <stdio.h>
#include <string.h>

int f_get_positive(string prompt);

int main(void)
{
    
    char brick[20] = "";
    char space[20] = "";
    strcpy(brick,"#");
    int n = f_get_positive("Enter positive value 1 to 8:\n");
    for (int i = 0; i < n; i++)
    {
        strcpy(space,"");
        for (int j = i; j < n - 1; j++)
        {
            strcat(space," ");
        }
        printf("%s%s  %s\n", space, brick, brick);
        strcat(brick, "#");
    }
}

int f_get_positive(string prompt)
{
    int hash;
    do 
    {
        hash = get_int("%s", prompt);
    }
    while (hash < 1 || hash > 8);
    
    return hash;
}
