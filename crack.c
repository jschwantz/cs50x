#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    // argc always == 1 with the name of the file; first parameter is 2
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    char salt[2];
    
    // set the salt
    salt[0] = argv[1][0];
    salt[1] = argv[1][1];
    
    char let[] = "AaIi";
    
    // try the single character items
    for (int i = 0; i < strlen(let); i++)
    {
        string pass = crypt(&let[i], salt);
        if (pass == argv[1])
        {
            return let[i];
        }
    }
    
    char test2[5] = "aa";
    char *pass2;
    // for length of word
    for (int i = 1; i < 5; i++)
    {
        test2[0] = 'a';
        test2[1] = 'a';
        test2[2] = 'a';    
        test2[3] = 'a';    
        test2[4] = 'a';    
        // for index 4
        for (int j = 0; j <= ('z' - 'A'); j++)
        {
            if (j > 'Z' - 'A' && j < 'a' - 'A')
                {
                    j += ('a' - 'Z' - 1);
                }
            if (i < 4 && j == 0)
            {
                j += ('z' - 'A');
            }
            // for index 3
            for (int k = 0; k <= ('z' - 'A'); k++)
            {
                if (k > 'Z' - 'A' && k < 'a' - 'A')
                {
                    k += ('a' - 'Z' - 1);
                }
                if (i < 3 && k == 0)
                {
                    k += ('z' - 'A');
                }
                // for index 2
                for (int l = 0; l <= ('z' - 'A'); l++)
                {
                    if (l > 'Z' - 'A' && l < 'a' - 'A')
                    {
                        l += ('a' - 'Z' - 1);
                    }
                    if (i < 2 && l == 0)
                    {
                        l += ('z' - 'A');
                    }
                        // for index 1
                    for (int m = 0; m <= ('z' - 'A'); m++)
                    {
                        if (m > 'Z' - 'A' && m < 'a' - 'A')
                        {
                            m += ('a' - 'Z' - 1);
                        }
                        // for incrementing letter, index 0
                        for (int n = 0; n <= ('z'-'A'); n++)
                        {
                            if (n > 'Z' - 'A' && n < 'a' - 'A')
                            {
                                n += ('a' - 'Z' - 1);
                            }
                            char testarg[5];
                            if (i == 1)
                            {
                                testarg[0] = test2[0];
                                testarg[1] = test2[1];
                            }
                            else
                            {
                             strncpy(testarg, test2, i+1);   
                            }
                            
                            pass2 = crypt(testarg, salt);
                            //if (testarg[0] == 'L' && testarg[1] == 'O')
                            //{
                                //printf("'%s'\n", testarg);
                                //printf("'%s'\n", pass2);
                                //printf("'%s'\n", argv[1]);
                            //}
                                
                            if (strcmp(pass2, argv[1]) == 0)
                            {
                                if (i == 1)
                                {
                                    printf("%c%c",testarg[0],testarg[1]);
                                }
                                else
                                {
                                    printf("%s\n", testarg);
                                }
                                return 0;
                            }
                            //printf("%s\n", test2);
                            if (test2[0]=='z')
                            {
                                test2[0] = 'A';
                            }
                            else 
                            {
                                test2[0]++;
                            }
                        }
                        if (test2[1]=='z')
                        {
                            test2[1] = 'A';
                        }
                        else 
                        {
                            test2[1]++;
                        }
                        test2[0]='a';
                    }
                    if (test2[2]=='z')
                    {
                        test2[2] = 'A';
                    }
                    else 
                    {
                        test2[2]++;
                    }
                    test2[1]='a';
                }
                if (test2[3]=='z')
                {
                    test2[3] = 'A';
                }
                else 
                {
                    test2[3]++;
                }
                test2[2]='a';
            }
            if (test2[4]=='z')
            {
                test2[4] = 'A';
            }
            else 
            {
                test2[4]++;
            }
            test2[3]='a';
        }
    }
    
    return 0;
}
