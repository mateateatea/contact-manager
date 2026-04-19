#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "logic.h"
#include "io.h"
#include "ui.h"
#include "validation.h"


int main(){

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    struct ContactArray my_book;
    contact_array_init(&my_book);

   if (contact_array_init(&my_book) == 0)
    { 
        printf("Memory allocation error!\n");
        return 1;
    }

    int choice;

    printf("What do you want to do?\n");
    printf("[0] Add contact\n");
    printf("[1] Show contacts\n");
    scanf("%d", &choice);

    if (choice == 0)
    {
        contact_add(&my_book);

        zapisDoPliku(&my_book, "src/contacts.csv");
    }
    if (choice == 1)
    {
        contact_show();
    }

    free(my_book.data);
    
    return 0;
}