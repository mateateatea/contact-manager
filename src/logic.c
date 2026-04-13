#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>

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
    scanf("%9s", nowy->phone);
    printf("Enter your e-mail: ");
    scanf("%99s", nowy->email);
    getchar();
    printf("Enter your home addres: ");
    
    fgets(nowy->address, sizeof(nowy->address), stdin);
    nowy->address[strcspn(nowy->address, "\n")] = 0;

    printf("Dodaj notatke (opcjonalne): ");
    fgets(nowy->note, sizeof(nowy->note), stdin);
    nowy->note[strcspn(nowy->note, "\n")] = 0;

    nowy -> date_added = time(NULL);

    arr -> size++;

    printf("The contact has been added!\n");

    return 1;
}

int contact_array_init(struct ContactArray *arr){
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
    arr->data = malloc(arr->capacity * sizeof(struct Contact));

    if (arr->data == NULL){
        return 0;
    }
    return 1;
}