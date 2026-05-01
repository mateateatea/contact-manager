#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "logic_gui.h"

int contact_add_gui(struct ContactArray *arr, 
                    const char *first_name, 
                    const char *last_name, 
                    const char *phone, 
                    const char *email, 
                    const char *address, 
                    const char *note) 
{
    // 1. Zarządzanie pamięcią (Twój oryginalny, świetny kod)
    if (arr->size >= arr->capacity) {
        struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
        if (tmp == NULL) {
            // W GUI nie używamy printf. Zwracamy kod błędu (0), 
            // a ui.c zdecyduje, czy wyświetlić czerwony komunikat na ekranie.
            return 0; 
        }
        arr->data = tmp;
        arr->capacity = arr->capacity * 2;
    }

    struct Contact *nowy = &arr->data[arr->size];

    // 2. Kopiowanie danych do struktury z zabezpieczeniem przed przepełnieniem bufora (Buffer Overflow)
    // Używamy strncpy zamiast strcpy
    
    strncpy(nowy->first_name, first_name, sizeof(nowy->first_name) - 1);
    nowy->first_name[sizeof(nowy->first_name) - 1] = '\0';

    strncpy(nowy->last_name, last_name, sizeof(nowy->last_name) - 1);
    nowy->last_name[sizeof(nowy->last_name) - 1] = '\0';

    strncpy(nowy->phone, phone, sizeof(nowy->phone) - 1);
    nowy->phone[sizeof(nowy->phone) - 1] = '\0';

    strncpy(nowy->email, email, sizeof(nowy->email) - 1);
    nowy->email[sizeof(nowy->email) - 1] = '\0';

    strncpy(nowy->address, address, sizeof(nowy->address) - 1);
    nowy->address[sizeof(nowy->address) - 1] = '\0';

    strncpy(nowy->note, note, sizeof(nowy->note) - 1);
    nowy->note[sizeof(nowy->note) - 1] = '\0';

    // 3. Ustawienie metadanych i inkrementacja rozmiaru
    nowy->date_added = time(NULL);
    arr->size++;

    return 1; // Sukces
}