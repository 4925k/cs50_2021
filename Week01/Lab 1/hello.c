#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask user to input name
    string userName = get_string("Enter Name: ");
    //Greet the user
    printf("Hello, %s\n", userName);
}