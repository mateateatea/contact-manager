#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "logic.h"
#include "io.h"
#include "ui.h"
#include "validation.h"

int main(){
    struct ContactArray moja_ksiazka;
    
    moja_ksiazka.size = 0;
    moja_ksiazka.capacity = INITIAL_CAPACITY;

    moja_ksiazka.data = malloc(moja_ksiazka.capacity * sizeof(struct Contact));

    if (moja_ksiazka.data == NULL)
    {
        printf("Błąd alokacji pamięci!\n");
        return 1;
    }

    contact_add(&moja_ksiazka);

    free(moja_ksiazka.data);
    
    return 0;
}