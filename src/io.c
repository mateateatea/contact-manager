#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "structs.h"

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

    printf("Data have been added to file %s\n", nazwa_pliku);
}