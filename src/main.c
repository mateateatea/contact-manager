#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "logic.h"
#include "io.h"
#include "ui.h"
#include "validation.h"


int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    struct ContactArray my_book;
    contact_array_init(&my_book);

    

   if (contact_array_init(&my_book) == 0){
    printf("Memory allocation error!\n");
    return 1;
}

    contact_add(&my_book);

    zapisDoPliku(&my_book, "src/contacts.csv");

    free(my_book.data);
    
    return 0;
}