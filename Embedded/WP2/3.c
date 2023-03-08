// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 2
// Exercise 3
// Submission Code: 29920

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// -----typedefs -------
typedef struct
{
    char firstName[20];
    char lastName[30];
    char pers_number[13]; // yyyymmddnnnc
} PERSON; //Decleare the type 'person'

char fileName[20] = {"personList.bin"};

// Declering our functions
void createList(PERSON *inrecord); // Creates list and adds dummy person
PERSON postPerson();   // Creates person and adds to file
void findPerson(); //Find people by first or lastname.
void printFile(void); //Print full list

int main(void)
{
    // Print out the ''Menu'' to instruct the user
    printf("1. Create a new and delete the old file.\n");
    printf("2. Add a new person to the file.\n");
    printf("3. Search for a person in the file.\n");
    printf("4. Print out all in the file.\n");
    printf("5. Exit the program.\n:");

    int x; // Decleare varible for the switch case.
    PERSON person;
    PERSON dummy = {"Carl", "Dahlqvist", "19970202"}; // Dummy data used in the createfile() to populate file

    scanf("%d", &x); // Take user input for the case to be runned

    switch (x) // Switch cases
    {
    case 1: //If case 1 is selected we run createlist()
        printf("You selected option 1.\n");
        createList(&dummy); //Function call
        main(); //Used to return to the menu
        break;
    case 2: //If case 1 is selected we run postPerson()
        printf("You selected option 2.\n");
        postPerson(); //Function call
        main(); //Used to return to manu

        break;
    case 3:
        printf("You selected option 3.\n");
        findPerson();
        main();
        break;
    case 4:
        printf("You selected option 4.\n");
        printFile();
        main();

        break;
    case 5:
        printf("Program ended! Good Bye! <3\n");
        exit(1); // Exits the program
        break;
    }
    return 0;
}

void createList(PERSON *inrecord) //Create our binary list
{
    FILE *file = fopen(fileName, "wb"); //create/open file
    if (file == NULL)
    {
        printf("Error opening file!\n");
    }
    fwrite(inrecord, sizeof(PERSON), 1, file); // write to file the dummydata person
    printf("You have created a new binary file named : %s \n", fileName); // Infrom the user
    fclose(file);                                                         // We close the file
}

PERSON postPerson()
{
    char firstName[20];   // Decleare variable of first name
    char lastName[20];    // Decleare variable of last name
    char pers_number[13]; // Decleare variable of personal Number
    PERSON person;        // Use the struct to define person

    printf("Enter first name: "); // Instruct the user
    scanf("%s", firstName);       // Take user input

    printf("Enter family name: "); // Instruct the user
    scanf("%s", lastName);         // Take user input

    printf("Enter personal number: "); // Instruct the user
    scanf("%s", pers_number);          // Take user input

    //Since strings are just arrays with chars we need to use strncpy().
    strncpy(person.firstName, firstName, 20);     // stringcopy on userinput
    strncpy(person.lastName, lastName, 20);       // stringcopy on userinput
    strncpy(person.pers_number, pers_number, 13); // stringcopy on userinput

    FILE *file = fopen(fileName, "ab"); // we open the file for append and read in binary
    if (file == NULL)                   //
    {
        printf("Error");
    }
    fwrite(&person, sizeof(PERSON), 1, file); // We write to the file the person we passed from the addperson()
    fclose(file);                             // closes file

    return person; // Return person so that it can be used in addToFile function.
}



// Search function to find names in the list
void findPerson()
{
    PERSON match;
    int i = 0;
    char name[20];  // Decleare the varible which we will use to search
    char option[1]; // Declere the varible for the option.

    printf("Enter '1' to search by firstName or enter '2' to search by lastName:"); // Instruct user with options
    scanf("%s", option);                                                            // Take user input for option

    if (option[0] == '1' || option[0] == '2') // Check if the input is either '1' or '2'
    {
        printf("Enter name of search: "); // Instruct user
        scanf("%s", name);             // Take user input for the name
        FILE *file = fopen(fileName, "rb"); // We open the file and read from it.

        if (option[0] == '1')             // If input is '1', we search by first name.
        {

            while (i == 0)
            {
                fread(&match, sizeof(PERSON), 1, file); //Read from the file
                if (!feof(file)) //While end of file not reached we countinue do string comperrsion
                {
                    if (strcmp(match.firstName, name) == 0) //Compear every firstname to user input (name)
                    { //if match we print the first,lastname and prnr.
                        printf("PersonalNr: %s\n FullName: %s %s\n", match.pers_number, match.firstName, match.lastName);
                    }
                }
                else
                {
                    i = EOF; // when reaching end of file.
                }
            }
            fclose(file);
        }
        else if (option[0] == '2') // If input i '2', we search by lastname.
        {
            while (i == 0)
            {
                fread(&match, sizeof(PERSON), 1, file);
                if (!feof(file))
                {
                    if (strcmp(match.lastName, name) == 0)
                    {
                        printf("PersonalNr: %s\nFullName: %s %s\n", match.pers_number, match.firstName, match.lastName);
                    }
                }
                else
                {
                    i = EOF;
                }
            }
            fclose(file);
        }
        else
        {
            printf("Invalid option"); // Imform about invalid input.
        }
    }
}

void printFile(void) //Prints all the persons in our file
{
    PERSON person;
    int i = 0;

    FILE *fileToPrint = fopen(fileName, "rb"); // We open the file for reading binary
    if (fileToPrint == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    while (!feof(fileToPrint)) //While it's not the end of file we countinue printing.
    {
        if (fread(&person,sizeof(PERSON),1,fileToPrint)){ // We read from the file and print out the pers_number, first and last name
            printf("PersonalNr: %s \nFullName: %s %s\n", person.pers_number, person.firstName, person.lastName);
            i++;
        }
    }

    fclose(fileToPrint);// we cloese the file.
}
