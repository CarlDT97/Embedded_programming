// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 1
// Exercise 5
// Submission Code 14620

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define MAXNUMBER 20

//-----Function Declarations-----

void create_random(int *tab);

void count_frequency(int *tab, int *freq);

void draw_histogram(int *freq);

//-----Function Behavior-----
void create_random(int *tab)
{
    // Initialize randomizer
    srand((unsigned)time(NULL));
    // For every index of the array of size MAX (100), assign a random number that is between 0 and MAXNUMBER (20)
    for (int i = 0; i < MAX; i++)
    {
        // Randomize a number at each index i
        tab[i] = rand() % MAXNUMBER;
    }
}

void count_frequency(int *tab, int *freq)
{
    // Checking each index of the table of size MAX
   for (int i = 0; i < MAX; i++)
   {
       // freq indexed by number from 0 to 19 as it is size 20. Each index will correspond to the frequency of said number. 
       // increment index of number when number occurs
       // eg if tab[5] contains 9, freq[9] will have its value incremented by 1.
       freq[tab[i]]++;
   }
}

void draw_histogram(int *freq)
{
    for (int i = 0; i <= MAXNUMBER; i++)
    {
        // double-check that the number occurs at least once.
        if (freq[i] >= 1)
        {
            // print number first
            printf("\n%d   ", i);
            for (int j = 0; j < freq[i]; j++)
            {
                // Print x for everytime the number occurs
                printf("%s","x");
            }
            // Add a newline character
            puts("");
        }
    }
}

int main(void)
{
    // initialize table
    int table[MAX];
    // create table
    create_random(table);
    // initialize frequency array
    int frequency[MAXNUMBER]={0}; 
    count_frequency(table, frequency);
    draw_histogram(frequency);
}
