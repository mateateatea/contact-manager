#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

void fillIt(char *buffer, size_t buffer_size)
{
    while (1)
    {
        if (fgets(buffer, buffer_size, stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strlen(buffer) == 0)
            {
                printf("You must fill it!\n");
                continue;
            }

            break;
        }
    }
}

int contact_add(struct ContactArray *arr)
{   
    if (arr->size >= arr->capacity){
        struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
        if (tmp == NULL){
            printf("Memory allocation error!\n");
            return 0;
        }
        arr->data = tmp;
        arr->capacity = arr->capacity * 2;
    }

    struct Contact *nowy = &arr->data[arr->size];

    int a;
    while ((a = getchar()) != '\n' && a != EOF);

    printf("Enter your name: ");
    fillIt(nowy->first_name, sizeof(nowy->first_name));

    printf("Enter your surname: ");
    fillIt(nowy->last_name, sizeof(nowy->last_name));

    printf("Enter your phone number: ");
    bool isValid;
    while (1)
    {
        if (fgets(nowy->phone, sizeof(nowy->phone), stdin) != NULL)
        {
            nowy->phone[strcspn(nowy->phone, "\n")] = 0;

            if (strlen(nowy->phone) == 0)
            {
                printf("You must fill the phone number!\n");
                continue;
            }
            
            isValid = true;

            for (int i = 0; nowy->phone[i] != '\0'; i++)
            {
                if (!isdigit(nowy->phone[i]))
                {
                    isValid = false;
                    break;
                }
            }

            if (isValid)
            {
                break;
            }
            else
            {
                printf("Wpisz tylko cyfry!\n");
            }
        }
    }
    
    printf("Enter your e-mail: ");
    
    while (1)
    {
        if (fgets(nowy->email, sizeof(nowy->email), stdin) != NULL)
        {
            nowy->email[strcspn(nowy->email, "\n")] = 0;

            if(strlen(nowy->email) == 0)
            {
                continue;
            }

            if (strchr(nowy->email, '@') != NULL)
            {
                break;
            }
            else
            {
            printf("Wrong email adress format! Try again!\n");;
            }
        }
    }

    printf("Enter your home addres: ");
    fillIt(nowy->address, sizeof(nowy->address));

    printf("Add note (optional): ");
    fgets(nowy->note, sizeof(nowy->note), stdin);
    nowy->note[strcspn(nowy->note, "\n")] = 0;

    nowy -> date_added = time(NULL);

    arr -> size++;

    printf("The contact has been added!\n");

    return 1;
}

int contact_array_init(struct ContactArray *arr)
{
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
    arr->data = malloc(arr->capacity * sizeof(struct Contact));

    if (arr->data == NULL){
        return 0;
    }
    return 1;
}

void contact_show()
{
    FILE* plik = fopen("src/contacts.csv", "r");
    if (!plik)
    {
        printf("File not found!");
    }
    else
    {
        char linie[MAX_LINE_LENGTH];

        int row = 0;
        int column = 0;


        while(fgets(linie, sizeof(linie), plik))
        {
            row++;
            column = 0;
            
            if (row == 1)
            {
                printf("First name, Last name, phone number, email address, home address, note, date\n");
                
            }
            else
            {
                char *token = strtok(linie, ",");
                while (token != NULL)
                {
                    printf("%-10s", token);

                    token = strtok(NULL, ",");
                }
                printf("\n");
                
            }
        }
        fclose(plik);
    }
}