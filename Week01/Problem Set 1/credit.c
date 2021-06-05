#include <stdio.h>
#include <cs50.h>

//prototype
bool checkLength(long card);
bool luhnAlgorithm(long card);
int cardType(long card);

int main(void)
{
    string result = "INVALID";
    //get card number from user
    long card = get_long("Number: ");

    //check length of the card
    if (checkLength(card))
    {
        if (luhnAlgorithm(card))
        {
            int initials = cardType(card);
            if (initials > 50 && initials < 56)
            {
                result = "MASTERCARD";
            }
            else if (initials == 34 || initials == 37)
            {
                result = "AMEX";
            }
            else if ((initials / 10) == 4)
            {
                result = "VISA";
            }
        }
    }

    printf("%s\n", result);
}


//function to check validity of the card
bool luhnAlgorithm(long card)
{
    //initialize required variables
    int count = 1;
    int sum1 = 0;
    int sum2 = 0;

    //steps to calculate sums
    while (card != 0)
    {
        int ln = card % 10;
        int type = card % 10;
        card = card / 10;

        if (count % 2 == 1)
        {
            sum1 = sum1 + ln;
        }
        else
        {
            ln = ln * 2;
            if (ln > 9)
            {
                ln = (ln - 10) + 1;
            }
            sum2 = sum2 + ln;
        }
        count++;
    }

    //cehck final sum and return results
    int totalSum = sum1 + sum2;
    if (totalSum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

int cardType(long card)
{
    while (card > 99)
    {
        card = card / 10;
    }
    return (int)card;
}

bool checkLength(long card)
{
    //check if card is 0
    if (card == 0)
    {
        return false;
    }

    //find out length of card
    int len = 0;
    while (card != 0)
    {
        card = card / 10;
        len++;
    }

    //check if card length if valid
    if (len < 13 || len > 16)
    {
        return false;
    }

    //return true if everything checks out
    return true;
}

