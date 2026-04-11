#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"
#include <string.h>

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
    nowy->address[strcspn(nowy->address, "\n")] = 0;

    printf("Dodaj notatke (opcjonalne): ");
    fgets(nowy->note, sizeof(nowy->note), stdin);
    nowy->note[strcspn(nowy->note, "\n")] = 0;

    nowy -> date_added = time(NULL);

    arr -> size++;

    printf("Kontakt został dodany!\n");

    return 1;
}

void zapisDoPliku(struct ContactArray *arr, const char* nazwa_pliku)
{
    FILE *plik = fopen(nazwa_pliku, "w");

    if (plik == NULL)
    {
        printf("Błąd: nie można otworzyć pliku %s\n", nazwa_pliku);
        return;
    }

    for (int i = 0; i < arr->size; i++)
    {
        struct Contact *k = &arr->data[i];

        struct tm *czas_info = localtime(&k->date_added);
        char sformatowana_data[20];
        
        strftime(sformatowana_data, sizeof(sformatowana_data), "%Y-%m-%d %H:%M", czas_info);
        

        fprintf(plik, "%s,%s,%s,%s,%s,%s,%s\n",
            k->first_name,
            k->last_name,
            k->phone,
            k->email,
            k->address,
            k->note,
            sformatowana_data);
    }

    fclose(plik);

    printf("Dane zostały zapisane do pliku %s\n", nazwa_pliku);
}