#include "../include/validation.h"
#include <string.h>
#include <ctype.h>

int is_name_valid(const char *name) {
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        char c = name[i];
        if (!isalpha(c) && c != '-' && c != ' ' && c != '\'') return 0;
    }
    return 1;
}

int is_phone_valid(const char *phone) {
    int len = strlen(phone);
    if (len == 9) {
        for (int i = 0; i < 9; i++) if (!isdigit(phone[i])) return 0;
        return 1;
    } else if (len == 12 && strncmp(phone, "+48", 3) == 0) {
        for (int i = 3; i < 12; i++) if (!isdigit(phone[i])) return 0;
        return 1;
    }
    return 0;
}

int is_email_valid(const char *email) {
    if (strlen(email) == 0) return 1; // Puste pole jest OK (opcjonalne)
    char *at_ptr = strchr(email, '@');
    if (at_ptr == NULL || at_ptr == email) return 0;
    char *dot_ptr = strchr(at_ptr, '.');
    if (dot_ptr == NULL || dot_ptr == at_ptr + 1 || *(dot_ptr + 1) == '\0') return 0;
    return 1;
}