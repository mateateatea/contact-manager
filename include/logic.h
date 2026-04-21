#ifndef LOGIC_H
#define LOGIC_H
#include "structs.h"

int contact_add(struct ContactArray *arr);
void contact_delete(struct ContactArray *arr);
void contact_edit(struct ContactArray *arr);
void contact_sort(struct ContactArray *arr);
int contact_search(struct ContactArray *arr, char *first_name, char *last_name);
void contact_show(struct ContactArray *arr);
void zapisDoPliku(struct ContactArray *arr, const char* nazwa_pliku);
int contact_array_init(struct ContactArray *arr);
void contact_load(struct ContactArray *arr, const char *filename);

#endif