#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //vairables to store population size
    int currentPopulation = 0;
    int targetPopulation = 0;
    //a loop to make sure user inputs population greater than 9
    while (currentPopulation < 9)
    {
        //ask user for current population
        currentPopulation = get_int("Start size: ");
    }
    //a loop to make sure the target population is greater than current
    while (targetPopulation < currentPopulation)
    {
        //get target population
        targetPopulation = get_int("End size: ");
    }
    //years saves the number of years required to reach
    //target population from current population
    int years = 0 ;

    //a loop to calculate the increase in population
    //it breaks when the current population reaches
    //the target population
    while (currentPopulation < targetPopulation)
    {
        //calculate number of new babies
        int birth = currentPopulation / 3;
        //calculate number of deaths
        int death = currentPopulation / 4;
        //update current population after births and deaths
        currentPopulation = currentPopulation + birth - death;
        years += 1;
    }

    //display number of years required
    printf("Years: %i \n", years);
}