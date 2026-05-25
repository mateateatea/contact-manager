#ifndef STRUCTS_H
#define STRUCTS_H
#include <time.h>

#define INITIAL_CAPACITY 10

struct Contact{
    char first_name[50];
    char last_name[50];
    char phone[13];
    char email[100];
    char address[200];
    char note[500];
    time_t date_added;
};

struct ContactArray{
    struct Contact *data;
    int size;
    int capacity;
};

#endif