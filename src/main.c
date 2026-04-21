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

   if (contact_array_init(&my_book) == 0)
    { 
        printf("Memory allocation error!\n");
        return 1;
    }

    contact_load(&my_book, "src/contacts.csv");

    int working = 1;

    while (working)
    {
        int choice;

        printf("What do you want to do?\n");
        printf("[1] Add contact\n");
        printf("[2] Show contacts\n");
        printf("[3] Delete contact\n");
        printf("[4] Edit contact\n");
        printf("[5] Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            contact_add(&my_book);
            zapisDoPliku(&my_book, "src/contacts.csv");
            break;

        case 2:
            contact_show(&my_book);
            break;

        case 3:
            contact_delete(&my_book);
            zapisDoPliku(&my_book, "src/contacts.csv");
            break;

        case 4:
            contact_edit(&my_book);
            zapisDoPliku(&my_book, "src/contacts.csv");
            break;

        case 5:
            working = 0;
            break;
        
        default:
            printf("Invalid choice!");
            break;
        }
    }

    free(my_book.data);
    
    return 0;
}