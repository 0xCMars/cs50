#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int nowPopulation = 0;
    do
    {
        nowPopulation = get_int("Start size: ");
    }
    while (nowPopulation < 9);

    // TODO: Prompt for end size
    int endPopulation = 0;
    do
    {
        endPopulation = get_int("End size: ");
    }
    while (endPopulation < nowPopulation);

    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    while (nowPopulation < endPopulation)
    {
        int born = nowPopulation / 3;
        int pass = nowPopulation / 4;
        nowPopulation = nowPopulation + born - pass;
        year++;
    }
    // TODO: Print number of years
    printf("Years: %i\n", year);
}