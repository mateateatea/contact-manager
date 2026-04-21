#include <stdio.h>
#include <time.h>
#include "logic.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

void fillIt(char *buffer, size_t buffer_size)
{
    while (1)
    {
        if (fgets(buffer, buffer_size, stdin) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strlen(buffer) == 0)
            {
                printf("You must fill it!\n");
                continue;
            }

            break;
        }
    }
}

void get_valid_phone(char *buffer, size_t size) {
    bool isValid;
    while (1) {
        if (fgets(buffer, size, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) == 0) {
                printf("You must fill the phone number!\n");
                continue;
            }
            
            isValid = true;
            for (int i = 0; buffer[i] != '\0'; i++) {
                if (!isdigit(buffer[i])) {
                    isValid = false;
                    break;
                }
            }
            if (isValid) break;
            else printf("Type only numbers!\n");
        }
    }
}

int contact_add(struct ContactArray *arr)
{   
    if (arr->size >= arr->capacity){
        struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
        if (tmp == NULL){
            printf("Memory allocation error!\n");
            return 0;
        }
        arr->data = tmp;
        arr->capacity = arr->capacity * 2;
    }

    struct Contact *nowy = &arr->data[arr->size];

    int a;
    while ((a = getchar()) != '\n' && a != EOF);

    printf("Enter your name: ");
    fillIt(nowy->first_name, sizeof(nowy->first_name));

    printf("Enter your surname: ");
    fillIt(nowy->last_name, sizeof(nowy->last_name));

    printf("Enter your phone number: ");
    get_valid_phone(nowy->phone, sizeof(nowy->phone));
    
    printf("Enter your e-mail: ");
    
    while (1)
    {
        if (fgets(nowy->email, sizeof(nowy->email), stdin) != NULL)
        {
            nowy->email[strcspn(nowy->email, "\n")] = 0;

            if(strlen(nowy->email) == 0)
            {
                continue;
            }

            if (strchr(nowy->email, '@') != NULL)
            {
                break;
            }
            else
            {
            printf("Wrong email adress format! Try again!\n");;
            }
        }
    }

    printf("Enter your home addres: ");
    fillIt(nowy->address, sizeof(nowy->address));

    printf("Add note (optional): ");
    fgets(nowy->note, sizeof(nowy->note), stdin);
    nowy->note[strcspn(nowy->note, "\n")] = 0;

    nowy -> date_added = time(NULL);

    arr -> size++;

    printf("The contact has been added!\n");

    return 1;
}

int contact_array_init(struct ContactArray *arr)
{
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
    arr->data = malloc(arr->capacity * sizeof(struct Contact));

    if (arr->data == NULL){
        return 0;
    }
    return 1;
}

char* get_csv_field(char **line_ptr)
{
    if (*line_ptr == NULL) return NULL;
    
    char *start = *line_ptr;
    char *comma = strchr(start, ',');
    
    if (comma)
    {
        *comma = '\0';
        *line_ptr = comma + 1;
    }
    else
    {
        *line_ptr = NULL;
    }
    return start;
}

void contact_load(struct ContactArray *arr, const char *filename)
{
    FILE *plik = fopen(filename, "r");
    if (!plik)
    {
        printf("Błąd otwierania pliku!\n");
        return;
    }

    char linie[MAX_LINE_LENGTH];
    int row = 0;

    while (fgets(linie, sizeof(linie), plik))
    {
        row++;

        linie[strcspn(linie, "\n")] = 0;

        if (arr->size >= arr->capacity)
        {
            struct Contact *tmp = realloc(arr->data, arr->capacity * 2 * sizeof(struct Contact));
            if (tmp == NULL)
            {
                printf("Memory allocation error!");
                fclose(plik);
                return;
            }
            arr->data = tmp;
            arr->capacity *= 2;
        }

        struct Contact *nowy = &arr->data[arr->size];

        char *ptr = linie;

        char *token = get_csv_field(&ptr);
        if (token) strcpy(nowy->first_name, token);

        token = get_csv_field(&ptr);
        if (token) strcpy(nowy->last_name, token);

        token = get_csv_field(&ptr);
        if (token) strcpy(nowy->phone, token);

        token = get_csv_field(&ptr);
        if (token) strcpy(nowy->email, token);

        token = get_csv_field(&ptr);
        if (token) strcpy(nowy->address, token);

        token = get_csv_field(&ptr);
        if (token) strcpy(nowy->note, token);

        token = get_csv_field(&ptr);
        if (token) 
        {
            struct tm tm_date = {0};
            int year, month, day, hour, minute;
            if (sscanf(token, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute) == 5) 
            {
                tm_date.tm_year = year - 1900;
                tm_date.tm_mon = month - 1;
                tm_date.tm_mday = day;
                tm_date.tm_hour = hour;
                tm_date.tm_min = minute;
                nowy->date_added = mktime(&tm_date);
            } 
            else 
            {
                nowy->date_added = 0;
            }
        }

        arr->size++;
    }
    fclose(plik);
    printf("Loaded %d contacts from file.\n", arr->size);
}

void contact_show(struct ContactArray *arr)
{
    if (arr->size == 0)
    {
        printf("The contact book is empty.\n");
        return;
    }

    printf("\n----CONTACT BOOK----\n");
    for (int i = 0; i < arr->size; i++)
    {
        struct Contact *c = &arr->data[i];

        char date_buf[20];
        struct tm *timeinfo = localtime(&c->date_added);
        if (timeinfo != NULL) {
            strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M", timeinfo);
        } else {
            strcpy(date_buf, "Unknown date");
        }

        printf("[%d] %s %s %s %s %s %s %s\n", 
               i, 
               c->first_name, 
               c->last_name, 
               c->phone, 
               c->email, 
               c->address, 
               c->note, 
               date_buf);
    }
    printf("----------\n");
}

void contact_delete(struct ContactArray *arr)
{
    int index;
    printf("Type index number of contact that you want delete: ");
    scanf("%d", &index);

    if (arr->size == 0)
    {
        printf("The contact book is empty");
        return;
    }

    if (index >= arr->size || index < 0)
    {
        printf("Index out of range!");
        return;
    }

    for (int i = index; i < arr->size - 1; i++)
    {
        arr->data[i] = arr->data[i+1];
    }

    arr->size--;

    printf("Contact deleted succesfully!\n");
}

void contact_edit(struct ContactArray *arr)
{
    int i;
    printf("Type index number of contact that you want to edit: ");
    scanf("%d", &i);

    if (i >= arr->size || i < 0)
    {
        printf("ERROR! Index out of range!");
        return;
    }

    struct Contact *c = &arr->data[i];

    char date_buf[20];
    struct tm *timeinfo = localtime(&c->date_added);
    if (timeinfo != NULL) 
    {
        strftime(date_buf, sizeof(date_buf), "%Y-%m-%d %H:%M", timeinfo);
    } 
    else 
    {
        strcpy(date_buf, "Unknown date");
    }

    printf("[%d] %s %s %s %s %s %s %s\n", 
        i, 
        c->first_name, 
        c->last_name, 
        c->phone, 
        c->email, 
        c->address, 
        c->note, 
        date_buf);

    printf("What do you want to edit?\n");
    printf("[1] Everything\n");
    printf("[2] First name\n");
    printf("[3] Last name\n");
    printf("[4] Phone number\n");
    printf("[5] Email\n");
    printf("[6] Address\n");
    printf("[7] Note\n");
    printf("Choice: ");
    int choice;
    scanf("%d", &choice);

    int a;
    while ((a = getchar()) != '\n' && a != EOF);

    switch (choice)
    {
        case 1:
            printf("Enter first name: ");
            fillIt(c->first_name, sizeof(c->first_name));

            printf("Enter last name: ");
            fillIt(c->last_name, sizeof(c->last_name));

            printf("Enter phone number: ");
            get_valid_phone(c->phone, sizeof(c->phone));
            
            printf("Enter an e-mail: ");
            
            while (1)
            {
                if (fgets(c->email, sizeof(c->email), stdin) != NULL)
                {
                    c->email[strcspn(c->email, "\n")] = 0;

                    if(strlen(c->email) == 0)
                    {
                        continue;
                    }

                    if (strchr(c->email, '@') != NULL)
                    {
                        break;
                    }
                    else
                    {
                    printf("Wrong email adress format! Try again!\n");;
                    }
                }
            }

            printf("Enter home addres: ");
            fillIt(c->address, sizeof(c->address));

            printf("Add note (optional): ");
            fgets(c->note, sizeof(c->note), stdin);
            c->note[strcspn(c->note, "\n")] = 0;

            c -> date_added = time(NULL);

            printf("The contact has been edited!\n");

            break;

        case 2:
            printf("Change first name: ");
            fillIt(c->first_name, sizeof(c->first_name));
            break;

        case 3:
            printf("Change last name: ");
            fillIt(c->last_name, sizeof(c->last_name));
            break;

        case 4:
            printf("Change phone number: ");
            get_valid_phone(c->phone, sizeof(c->phone));
            break;

        case 5:
            printf("Change an e-mail: ");
            
            while (1)
            {
                if (fgets(c->email, sizeof(c->email), stdin) != NULL)
                {
                    c->email[strcspn(c->email, "\n")] = 0;

                    if(strlen(c->email) == 0)
                    {
                        continue;
                    }

                    if (strchr(c->email, '@') != NULL)
                    {
                        break;
                    }
                    else
                    {
                    printf("Wrong email adress format! Try again!\n");;
                    }
                }
            }
            break;

        case 6:
            printf("Change home addres: ");
            fillIt(c->address, sizeof(c->address));
            break;

        case 7:
            printf("Change note: ");
            fgets(c->note, sizeof(c->note), stdin);
            c->note[strcspn(c->note, "\n")] = 0;
            break;

        default:
            printf("Invalid input!\n");
            break;
    }
}