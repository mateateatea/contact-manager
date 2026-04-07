#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"

int contact_add(struct ContactArray *arr)
{
    if (arr->size >= arr->capacity) {
        printf("Brak miejsca! Tablica jest pelna.\n");
        return 0; 
    }

    struct Contact *nowy = &arr->data[arr->size];

    printf("Podaj imie: ");
    scanf("%49s", nowy->first_name);
    printf("Podaj nazwisko: ");
    scanf("%49s", nowy->last_name);
    printf("Podaj nr telefonu: ");
    scanf("%9s", nowy->phone);
    printf("Podaj adres e-mail: ");
    scanf("%99s", nowy->email);
    printf("Podaj adres zamieszkania: ");
    getchar();
    fgets(nowy->address, sizeof(nowy->address), stdin);

    printf("Dodaj notatke (opcjonalne): ");
    fgets(nowy->note, sizeof(nowy->note), stdin);

    nowy -> date_added = time(NULL);

    arr -> size++;

    printf("Kontakt został dodany!\n");

    return 1;
}