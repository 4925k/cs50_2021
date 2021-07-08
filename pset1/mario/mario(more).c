#include <stdio.h>
#include <cs50.h>

//prototype
void mario(int height);
void hashTree(int n);

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height >8);

    mario(height);
}

void mario(int height)
{
    //to work on 8 lines
    for (int i = 0; i < height; i++)
    {
        //left white space
        for (int j = (height - 1); j > i; j--)
        {
            printf(" ");
        }

        //left hashes
        hashTree(i);

        //middle part
        printf("  ");

        //right hashes
        hashTree(i);

        printf("\n");
    }
}

void hashTree(int n)
{
    for (int k = 0; k <= n ; k++)
    {
        printf("#");
}