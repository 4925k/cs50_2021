#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool checkKey(string key);
string substitute(string key, string text);

int main(int argc, string argv[])
{
    if (argc != 2) //check for key
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26) //check length of key
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!(checkKey(argv[1]))) //check if key is valid
    {
        printf("The key is invalid.\n");
        return 1;
    }

    //ask for plain text
    string plaintext = get_string("plaintext: ");

    //convert to encrypted text
    string ciphertext = substitute(argv[1], plaintext);

    //output encrypted text
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

string substitute(string key, string text)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (islower(text[i]))
        {
            text[i] = key[(int)text[i] - 97];
            text[i] = tolower(text[i]);
        }
        else if (isupper(text[i]))
        {
            text[i] = key[(int)text[i] - 65];
        }
    }
    return text;
}


bool checkKey(string key)
{
    string temp = key;
    //check if key has alphabets only
    for (int i = 0; i < 26; i++)
    {
        temp[i] = toupper(key[i]);
        if (!(temp[i] >= 65 && temp[i] <= 90))
        {
            return false;
        }
        //check for duplicates
        for (int j = 0; j < i; j++)
        {
            if (temp[i] == temp[j])
            {
                return false;
            }
        }
    }
    return true;
}