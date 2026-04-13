#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

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

    printf("Enter your name: ");
    scanf("%49s", nowy->first_name);
    printf("Enter your surname: ");
    scanf("%49s", nowy->last_name);
    printf("Enter your phone number: ");
    char tel[9];
    bool isValid;
    while (1)
    {
        fgets(nowy->phone, sizeof(nowy->phone), stdin);

        if (nowy->phone != NULL)
        {
            nowy->phone[strcspn(nowy->phone, "\n")] = 0;

            if (strlen(nowy->phone) == 0)
            {
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
    scanf("%99s", nowy->email);
    getchar();
    printf("Enter your home addres: ");
    
    fgets(nowy->address, sizeof(nowy->address), stdin);
    nowy->address[strcspn(nowy->address, "\n")] = 0;

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