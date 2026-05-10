#ifndef LOGIC_GUI_H
#define LOGIC_GUI_H

#include "structs.h"
#include <time.h>

int contact_array_init_gui(struct ContactArray *arr);

// Wczytywanie z pliku CSV (zwraca liczbę wczytanych kontaktów lub -1 jeśli brak pliku)
int contact_load_gui(struct ContactArray *arr, const char *filename);

// Dodawanie nowego kontaktu (zwraca 1 - sukces, 0 - błąd alokacji)
int contact_add_gui(struct ContactArray *arr, 
                    const char *first_name, 
                    const char *last_name, 
                    const char *phone, 
                    const char *email, 
                    const char *address, 
                    const char *note);

int contact_edit_gui(struct ContactArray *arr, 
                     int index,
                     const char *first_name, 
                     const char *last_name, 
                     const char *phone, 
                     const char *email, 
                     const char *address, 
                     const char *note);

// Usuwanie kontaktu (zwraca 1 - sukces, 0 - zły indeks)
int contact_delete_gui(struct ContactArray *arr, int index);

// Sortowanie alfabetyczne
void contact_sort_gui(struct ContactArray *arr);

// Wyszukiwanie (zwraca indeks w tablicy lub -1 jeśli nie znaleziono)
int contact_search_gui(struct ContactArray *arr, const char *first_name, const char *last_name);

// Zapisuje kontakty do pliku. Zwraca 1 (sukces) lub 0 (błąd)
int contact_save_gui(struct ContactArray *arr, const char *filename);

#endif