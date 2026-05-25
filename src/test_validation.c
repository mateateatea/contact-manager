#include <stdio.h>
#include <assert.h>
#include "../include/validation.h"

int main() {
    printf("--- ROZPOCZYNAM TESTY JEDNOSTKOWE FORMULARZA ---\n");

    // 1. Testy Imienia / Nazwiska
    assert(is_name_valid("Adam") == 1);
    assert(is_name_valid("Anna-Maria") == 1);
    assert(is_name_valid("Adam123") == 0); // Błąd: cyfry
    assert(is_name_valid("") == 0);        // Błąd: puste pole

    // 2. Testy Telefonu
    assert(is_phone_valid("123456789") == 1);
    assert(is_phone_valid("+48123456789") == 1);
    assert(is_phone_valid("12345678") == 0);       // Błąd: za krótki
    assert(is_phone_valid("+4812345678a") == 0);   // Błąd: zawiera literę

    // 3. Testy Email
    assert(is_email_valid("test@domena.pl") == 1);
    assert(is_email_valid("test@domena") == 0);    // Błąd: brak kropki/domeny
    assert(is_email_valid("testdomena.pl") == 0);  // Błąd: brak @
    assert(is_email_valid("") == 1);               // Puste pole (jest opcjonalne)

    printf("Wszystkie 12 testow zakonczone pelnym SUKCESEM!\n");
    return 0;
}