#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int colemanLiauIndex(string text);

int main(void)
{
    string text = get_string("Text: ");
    int result = colemanLiauIndex(text);
    if (result > 16)
    {
        printf("Grade 16+\n");
    }
    else if (result < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", result);
    }
}


int countLetters(string text)
{
    //count will store number of letters
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        text[i] = toupper(text[i]);
        if ((int)text[i] > 64 && (int)text[i] < 91) //count letters
        {
            count++;
        }
    }
    return count;
}

int countWords(string text)
{
    //count will store number of words
    int count = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        text[i] = toupper(text[i]);
        if ((int)text[i] == 32)  //count words
        {
            count++;
        }
    }
    //incremented count to count the last word
    return count;
}

int countSentences(string text)
{
    //count will store number of sentences.
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        text[i] = toupper(text[i]);
        if ((int)text[i] == 46 || (int)text[i] == 33 || (int)text[i] == 63)
        {
            count++;
        }//count words
    }
    return count;
}


int colemanLiauIndex(string text)
{
    //fetch required values
    int letters = countLetters(text);
    int words = countWords(text);
    int sentences = countSentences(text);


    //calculate averages
    float averageLetters = (float)letters / words * 100;
    float averageSentences = (float)sentences / words * 100;

    //calculate coleman-liau index
    float index = 0.0588 * averageLetters - 0.296 * averageSentences - 15.8;

    //rounding to the nearest integer
    int result = (index < ((int)index + 0.5)) ? (int)index : (int)index + 1;

    //printf("%i %i %i %f %f %f\n", letters, words, sentences, averageLetters, averageSentences, index);

    return result;
}