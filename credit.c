#include <stdio.h>
#include <cs50.h>
#include <math.h>

long f_get_value(string prompt);
long len_value(long a_val);
long f_get_digit(long a_num, long a_dig);

int main(void)
{
    int sum = 0;
    long num = f_get_value("Please enter card number:\n");
    long length = len_value(num);
    
    if (length == 14 || length == -1)
    {
        //printf("Number:%s\n",num);
        printf("INVALID\n");
    }
    else
    {
        for (int i = length - 1; i > 0; i = i - 2)
        {
            int x = f_get_digit(num, i);
            if (2*x > 9)
            {
                int a = f_get_digit(2*x, 1);
                int b = f_get_digit(2*x, 2);
                sum += a + b;
                //printf("a%i,b%i\n", a,b);
                //printf("sum%i\n", sum);
            }
            else
            {
                sum += 2*x;
                //printf("x%i\n", x);
                //printf("sum%i\n", sum);
            }
        }
        
        for (int i = length; i > 0; i = i - 2)
        {
            int x = f_get_digit(num, i);
            sum += x;
            //printf("sum%i\n", sum);
        }
        
        int first = f_get_digit(num, 1);
        int sec = f_get_digit(num, 2);
        
        if (sum % 10 == 0 && (length == 15) && first == 3 && (sec == 4 || sec == 7))
        {
            printf("AMEX\n");
        }
        else if (sum % 10 == 0 && (length == 16 || length == 13) && first == 4)
        {
            printf("VISA\n");
        }
        else if (sum % 10 == 0 && length == 16 && first == 5 && (sec == 1 || sec == 2 || sec == 3 || sec == 4 || sec == 5))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
}

long f_get_value(string prompt)
{
    long n;
    n = get_long("%s", prompt);
    return n;
}

long f_get_digit(long a_num, long a_dig)
{
    int length = len_value(a_num);
    
    if (a_dig <= length - 1)
    {
        a_num = a_num / (pow(10,(length - a_dig)));
    }
    long x = a_num % 10;
    return x;
}

long len_value(long a_val)
{
    if (a_val < 10 && a_val > -1)
    {
        return 1;
    }
    else if (a_val < 100 && a_val > 9)
    {
        return 2;
    }
    else if (a_val < 10000000000000 && a_val > 999999999999)
    {
        return 13;
    }
    else if (a_val < 100000000000000 && a_val > 9999999999999)
    {
        return 14;
    }
    else if (a_val < 1000000000000000 && a_val > 99999999999999)
    {
        return 15;
    }
    else if (a_val < 10000000000000000 && a_val > 999999999999999)
    {
        return 16;
    }
    else 
    {
        return -1;
    }
}
