#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "logic_gui.h"
#include <stdio.h>

static char* get_csv_field(char **line_ptr)
{
    if (*line_ptr == NULL) return NULL;
    
    char *start = *line_ptr;
    char *comma = strchr(start, ',');
    
    if (comma)
    {
        *comma = '\0';
        *line_ptr = comma + 1;
    }
    else
    {
        *line_ptr = NULL;
    }
    return start;
}

int contact_array_init_gui(struct ContactArray *arr)
{
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY; // Upewnij się, że INITIAL_CAPACITY jest w structs.h
    arr->data = malloc(arr->capacity * sizeof(struct Contact));

    if (arr->data == NULL) {
        return 0; // Błąd
    }
    return 1; // Sukces
}

int contact_load_gui(struct ContactArray *arr, const char *filename)
{
    FILE *plik = fopen(filename, "r");
    if (!plik)
    {
        return -1; // Błąd otwarcia pliku
    }

    char linie[1024]; // MAX_LINE_LENGTH
    int count = 0;

    while (fgets(linie, sizeof(linie), plik))
    {
        linie[strcspn(linie, "\n")] = 0;

        if (arr->size >= arr->capacity)
        {
            struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
            if (tmp == NULL)
            {
                fclose(plik);
                return count; // Zwracamy ile udało się wczytać do momentu błędu
            }
            arr->data = tmp;
            arr->capacity *= 2;
        }

        struct Contact *nowy = &arr->data[arr->size];
        char *ptr = linie;

        char *token = get_csv_field(&ptr);
        if (token) strncpy(nowy->first_name, token, sizeof(nowy->first_name) - 1);

        token = get_csv_field(&ptr);
        if (token) strncpy(nowy->last_name, token, sizeof(nowy->last_name) - 1);

        token = get_csv_field(&ptr);
        if (token) strncpy(nowy->phone, token, sizeof(nowy->phone) - 1);

        token = get_csv_field(&ptr);
        if (token) strncpy(nowy->email, token, sizeof(nowy->email) - 1);

        token = get_csv_field(&ptr);
        if (token) strncpy(nowy->address, token, sizeof(nowy->address) - 1);

        token = get_csv_field(&ptr);
        if (token) strncpy(nowy->note, token, sizeof(nowy->note) - 1);

        token = get_csv_field(&ptr);
        if (token) 
        {
            struct tm tm_date = {0};
            int year, month, day, hour, minute;
            if (sscanf(token, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) == 5) 
            {
                tm_date.tm_year = year - 1900;
                tm_date.tm_mon = month - 1;
                tm_date.tm_mday = day;
                tm_date.tm_hour = hour;
                tm_date.tm_min = minute;
                nowy->date_added = mktime(&tm_date);
            } 
            else 
            {
                nowy->date_added = 0;
            }
        }

        arr->size++;
        count++;
    }
    
    fclose(plik);
    return count; // Sukces: zwraca liczbę wczytanych kontaktów
}

int contact_add_gui(struct ContactArray *arr, 
                    const char *first_name, 
                    const char *last_name, 
                    const char *phone, 
                    const char *email, 
                    const char *address, 
                    const char *note)
{   
    if (arr->size >= arr->capacity) {
        struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
        if (tmp == NULL) return 0; // Błąd alokacji
        
        arr->data = tmp;
        arr->capacity = arr->capacity * 2;
    }

    struct Contact *nowy = &arr->data[arr->size];

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

    nowy->date_added = time(NULL);

    arr->size++;
    return 1; // Sukces
}

int contact_edit_gui(struct ContactArray *arr, 
                     int index,
                     const char *first_name, 
                     const char *last_name, 
                     const char *phone, 
                     const char *email, 
                     const char *address, 
                     const char *note)
{
    if (index >= arr->size || index < 0) return 0; // Błąd: Zły indeks

    struct Contact *c = &arr->data[index];

    // W interfejsie graficznym przy edycji najłatwiej po prostu nadpisać wszystkie pola 
    // nowymi wartościami z buforów tekstowych (nawet jeśli część się nie zmieniła).
    strncpy(c->first_name, first_name, sizeof(c->first_name) - 1);
    c->first_name[sizeof(c->first_name) - 1] = '\0';

    strncpy(c->last_name, last_name, sizeof(c->last_name) - 1);
    c->last_name[sizeof(c->last_name) - 1] = '\0';

    strncpy(c->phone, phone, sizeof(c->phone) - 1);
    c->phone[sizeof(c->phone) - 1] = '\0';

    strncpy(c->email, email, sizeof(c->email) - 1);
    c->email[sizeof(c->email) - 1] = '\0';

    strncpy(c->address, address, sizeof(c->address) - 1);
    c->address[sizeof(c->address) - 1] = '\0';

    strncpy(c->note, note, sizeof(c->note) - 1);
    c->note[sizeof(c->note) - 1] = '\0';

    return 1; // Sukces
}

int contact_delete_gui(struct ContactArray *arr, int index)
{
    if (arr->size == 0) return 0;
    if (index >= arr->size || index < 0) return 0;

    // Przesuwanie elementów tablicy, aby załatać "dziurę" po usuniętym kontakcie
    for (int i = index; i < arr->size - 1; i++)
    {
        arr->data[i] = arr->data[i+1];
    }

    arr->size--;
    return 1; // Sukces
}

static int comparison_alphabetic(const void *a, const void *b)
{
    const struct Contact *contactA = (const struct Contact *)a;
    const struct Contact *contactB = (const struct Contact *)b;

    // Najpierw sortujemy po IMIENIU
    int name_cmp = strcmp(contactA->first_name, contactB->first_name);

    // Jeśli imiona są identyczne (np. dwóch Janów), sortujemy ich po NAZWISKU
    if (name_cmp == 0)
    {
        return strcmp(contactA->last_name, contactB->last_name);
    }
    
    return name_cmp;
}

void contact_sort_gui(struct ContactArray *arr)
{
    if (arr->size <= 1) return;
    qsort(arr->data, arr->size, sizeof(struct Contact), comparison_alphabetic);
}

int contact_search_gui(struct ContactArray *arr, const char *first_name, const char *last_name)
{
    if (arr == NULL || arr->data == NULL) return -1;

    for (int i = 0; i < arr->size; i++) {
        if (strcmp(arr->data[i].first_name, first_name) == 0 &&
            strcmp(arr->data[i].last_name, last_name) == 0) 
        {
            return i; // Znaleziono, zwraca indeks!
        }
    }
    return -1; // Nie znaleziono
}

int contact_save_gui(struct ContactArray *arr, const char *filename)
{
    FILE *plik = fopen(filename, "w"); // "w" nadpisuje plik nowymi danymi
    if (!plik) return 0; // Błąd otwarcia pliku

    for (int i = 0; i < arr->size; i++)
    {
        struct Contact *c = &arr->data[i];
        
        // Formatowanie daty do zapisu
        char date_buf[20] = "";
        if (c->date_added > 0) {
            struct tm *timeinfo = localtime(&c->date_added);
            if (timeinfo != NULL) {
                strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M", timeinfo);
            }
        }

        // Zapis do pliku CSV po przecinkach
        fprintf(plik, "%s,%s,%s,%s,%s,%s,%s\n",
                c->first_name,
                c->last_name,
                c->phone,
                c->email,
                c->address,
                c->note,
                date_buf);
    }

    fclose(plik);
    return 1; // Sukces
}