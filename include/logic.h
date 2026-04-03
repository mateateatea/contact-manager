#ifndef LOGIC_H
#define LOGIC_H
#include "structs.h"

int contact_add(struct ContactArray *arr);
int contact_delete(struct ContactArray *arr, int index);
int contact_edit(struct ContactArray *arr, int index, struct Contact new_data);
void contact_sort(struct ContactArray *arr);
int contact_search(struct ContactArray *arr, char *first_name, char *last_name);
void contact_show(struct ContactArray *arr);

#endif