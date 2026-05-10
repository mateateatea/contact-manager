#include <stdio.h>
#include <string.h> 
#include "ui.h"
#include "raylib.h" 
#include "structs.h"
#include "logic_gui.h" 
#include <ctype.h>

// Rozbudowane stany focusu (dla wszystkich 6 pól)
typedef enum { EKRAN_LISTY, EKRAN_DODAWANIA, EKRAN_EDYCJI } StanAplikacji;
typedef enum { FOCUS_NONE, FOCUS_FNAME, FOCUS_LNAME, FOCUS_PHONE, FOCUS_EMAIL, FOCUS_CITY, FOCUS_NOTE, FOCUS_SEARCH } FocusState;

// ... (w start_gui obok innych buforów):
char buf_search[50] = {0}; // Bufor na tekst z wyszukiwarki

void start_gui(struct ContactArray *my_book) {
    InitWindow(800, 600, "Contact Manager");
    SetTargetFPS(60);

    Font myFont = LoadFont("assets/Inter_18pt-Bold.ttf");

    Color bgColor = (Color){ 245, 245, 250, 255 };       
    Color cardColor = WHITE;                             
    Color textColor = (Color){ 40, 40, 40, 255 };        
    Color primaryColor = (Color){ 74, 144, 226, 255 };   

    StanAplikacji stan = EKRAN_LISTY;
    FocusState focus = FOCUS_NONE;

    float scroll_y = 0.0f;

    // Bufory dla 6 pól
    char buf_fname[50] = {0};
    char buf_lname[50] = {0};
    char buf_phone[20] = {0};
    char buf_email[50] = {0};
    char buf_city[50] = {0};
    char buf_note[100] = {0}; // Zrobimy ciut więcej miejsca na notatkę
    char error_msg[100] = {0}; 
    int edytowany_indeks = -1;

    while (!WindowShouldClose()) {
        
        // --- LOGIKA WPISYWANIA Z KLAWIATURY ---
        // --- LOGIKA WPISYWANIA Z KLAWIATURY ---
        char *active_buf = NULL;
        size_t max_len = 0;

        // Określamy, gdzie aktualnie pisze użytkownik
        if (stan == EKRAN_DODAWANIA || stan == EKRAN_EDYCJI) {
            if (focus == FOCUS_FNAME) { active_buf = buf_fname; max_len = 49; }
            else if (focus == FOCUS_LNAME) { active_buf = buf_lname; max_len = 49; }
            else if (focus == FOCUS_PHONE) { active_buf = buf_phone; max_len = 19; }
            else if (focus == FOCUS_EMAIL) { active_buf = buf_email; max_len = 49; }
            else if (focus == FOCUS_CITY)  { active_buf = buf_city; max_len = 49; }
            else if (focus == FOCUS_NOTE)  { active_buf = buf_note; max_len = 99; }
        } else if (stan == EKRAN_LISTY) {
            // NOWE: Pozwalamy pisać na ekranie głównym
            if (focus == FOCUS_SEARCH) { active_buf = buf_search; max_len = 49; }
        }

        // Łapanie klawiszy (ten sam kod co miałeś)
        if (active_buf != NULL) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (strlen(active_buf) < max_len)) {
                    size_t len = strlen(active_buf);
                    active_buf[len] = (char)key;
                    active_buf[len + 1] = '\0';
                }
                key = GetCharPressed(); 
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                size_t len = strlen(active_buf);
                if (len > 0) active_buf[len - 1] = '\0';
            }
        }

        // --- RYSOWANIE ---
        BeginDrawing();
        ClearBackground(bgColor);

        if (stan == EKRAN_LISTY) {
            DrawTextEx(myFont, "Twoje Kontakty", (Vector2){40, 30}, 36, 1, primaryColor);

            // --- NOWE: RYSOWANIE PASKA WYSZUKIWANIA ---
            Rectangle rec_search = { 320, 30, 220, 40 }; // Obok tytułu
            DrawRectangleRec(rec_search, cardColor);
            DrawRectangleLinesEx(rec_search, 2, (focus == FOCUS_SEARCH) ? primaryColor : LIGHTGRAY);
            
            // Wyświetlanie placeholdera (szarego "Szukaj...") lub wpisanego tekstu
            if (strlen(buf_search) == 0 && focus != FOCUS_SEARCH) {
                DrawTextEx(myFont, "Szukaj...", (Vector2){rec_search.x + 10, rec_search.y + 10}, 20, 1, GRAY);
            } else {
                DrawTextEx(myFont, buf_search, (Vector2){rec_search.x + 10, rec_search.y + 10}, 20, 1, textColor);
            }

            Rectangle btnAdd = { 560, 30, 200, 40 };
            DrawRectangleRounded(btnAdd, 0.2f, 10, primaryColor);
            DrawTextEx(myFont, "+ Dodaj Kontakt", (Vector2){btnAdd.x + 15, btnAdd.y + 10}, 20, 1, WHITE);

            // --- NOWE: KLIKANIE W PASEK WYSZUKIWANIA ---
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, btnAdd)) {
                    stan = EKRAN_DODAWANIA;
                    error_msg[0] = '\0'; 
                } else if (CheckCollisionPointRec(mouse, rec_search)) {
                    focus = FOCUS_SEARCH;
                } else {
                    focus = FOCUS_NONE; // Odznacza się, jeśli klikniesz w tło
                }
            }

            int startY = 90;       
            int cardHeight = 110;  
            int spacing = 15;      

            scroll_y += GetMouseWheelMove() * 30.0f; 
            if (scroll_y > 0) scroll_y = 0; 
            int total_height = my_book->size * (cardHeight + spacing);
            int max_scroll = 600 - startY - total_height;
            if (max_scroll > 0) max_scroll = 0; 
            if (scroll_y < max_scroll) scroll_y = max_scroll;

            BeginScissorMode(0, startY, 800, 600 - startY);

            // Zmienna trzymająca ilość AKTUALNIE narysowanych, widocznych kontaktów
            int drawn_count = 0; 

            for (int i = 0; i < my_book->size; i++) {
                
                // --- NOWE: LOGIKA WYSZUKIWANIA ---
                if (strlen(buf_search) > 0) {
                    // Robimy kopię małych liter wpisanego tekstu
                    char szukane[50];
                    strcpy(szukane, TextToLower(buf_search)); 
                    
                    // Sklejamy imię, nazwisko i telefon w jeden string do przeszukania
                    // i też zamieniamy na małe litery (żeby wpisanie "jan" znalazło "Jan")
                    const char *caly_tekst = TextFormat("%s %s %s", my_book->data[i].first_name, my_book->data[i].last_name, my_book->data[i].phone);
                    const char *dane_lower = TextToLower(caly_tekst);
                    
                    // Funkcja strstr sprawdza, czy wpisany kawałek występuje w danych
                    if (strstr(dane_lower, szukane) == NULL) {
                        continue; // Jeśli nie znaleziono, omijamy ten kontakt!
                    }
                }

                // OBLICZANIE POZYCJI (Zauważ, że używamy 'drawn_count' a nie 'i')
                int currentY = startY + (int)scroll_y + (drawn_count * (cardHeight + spacing));
                
                if (currentY + cardHeight < startY) {
                    drawn_count++;
                    continue; 
                }
                if (currentY > 600) break;

                // --- STARE RYSOWANIE KAFELKA ---
                Rectangle cardRec = {40, currentY, 720, cardHeight};
                DrawRectangleRounded(cardRec, 0.2f, 10, cardColor);
                DrawRectangleRoundedLines(cardRec, 0.2f, 10, LIGHTGRAY); 

                const char *fullName = TextFormat("%s %s", my_book->data[i].first_name, my_book->data[i].last_name);
                DrawTextEx(myFont, fullName, (Vector2){60, currentY + 15}, 24, 1, textColor);

                const char *phone = TextFormat("Tel: %s", my_book->data[i].phone);
                DrawTextEx(myFont, phone, (Vector2){60, currentY + 45}, 18, 1, GRAY);

                const char *email = TextFormat("Email: %s", my_book->data[i].email);
                DrawTextEx(myFont, email, (Vector2){400, currentY + 15}, 18, 1, GRAY);

                const char *city = TextFormat("Miasto: %s", my_book->data[i].address);
                DrawTextEx(myFont, city, (Vector2){400, currentY + 45}, 18, 1, GRAY);

                const char *note = TextFormat("Notatka: %s", my_book->data[i].note);
                DrawTextEx(myFont, note, (Vector2){60, currentY + 75}, 16, 1, DARKGRAY);

                Rectangle btnDelete = { 660, currentY + 35, 80, 40 }; // Pozycja guzika
                
                // Rysujemy czerwony prostokąt i biały napis
                DrawRectangleRounded(btnDelete, 0.2f, 10, RED);
                DrawTextEx(myFont, "Usun", (Vector2){btnDelete.x + 15, btnDelete.y + 10}, 18, 1, WHITE);

                // Obsługa kliknięcia w ten konkretny przycisk
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnDelete)) {
                    
                    // 1. Usuwamy kontakt z pamięci (przesuwa tablicę)
                    contact_delete_gui(my_book, i);
                    
                    // 2. Automatycznie zapisujemy nową, mniejszą listę do pliku!
                    contact_save_gui(my_book, "src/contacts.csv"); 
                    
                    // 3. PRZERYWAMY PĘTLĘ! 
                    // To bardzo ważne: usunęliśmy element i przesunęliśmy tablicę. 
                    // Dalsze rysowanie w tej klatce (frame) mogłoby spowodować błąd z indeksami. 
                    // Pętla narysuje listę poprawnie od nowa w ułamku sekundy.
                    break; 
                }

                // --- PRZYCISK EDYCJI (Obok usuwania) ---
                Rectangle btnEdit = { 570, currentY + 35, 80, 40 }; 
                DrawRectangleRounded(btnEdit, 0.2f, 10, GREEN);
                DrawTextEx(myFont, "Edytuj", (Vector2){btnEdit.x + 10, btnEdit.y + 10}, 18, 1, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnEdit)) {
                    stan = EKRAN_EDYCJI;
                    edytowany_indeks = i; // Zapisujemy, kogo edytujemy!
                    error_msg[0] = '\0';
                    
                    // Wypełniamy bufory danymi tego kontaktu, żeby można było je poprawiać
                    strcpy(buf_fname, my_book->data[i].first_name);
                    strcpy(buf_lname, my_book->data[i].last_name);
                    strcpy(buf_phone, my_book->data[i].phone);
                    strcpy(buf_email, my_book->data[i].email);
                    strcpy(buf_city, my_book->data[i].address);
                    strcpy(buf_note, my_book->data[i].note);
                }
                
               

                drawn_count++; // Zwiększamy liczbę widocznych kontaktów
            }

            EndScissorMode();
        } 
        else if (stan == EKRAN_DODAWANIA || stan == EKRAN_EDYCJI) {
            if (stan == EKRAN_DODAWANIA) {
                DrawTextEx(myFont, "Dodaj Nowy Kontakt", (Vector2){40, 30}, 36, 1, primaryColor);
            } else {
                DrawTextEx(myFont, "Edytuj Kontakt", (Vector2){40, 30}, 36, 1, primaryColor);
            }

            // 1. Dwie kolumny dla pół tekstowych (Lewa: x=40, Prawa: x=400)
            Rectangle rec_fname = { 40, 120, 320, 40 };
            Rectangle rec_lname = { 40, 200, 320, 40 };
            Rectangle rec_phone = { 40, 280, 320, 40 };

            Rectangle rec_email = { 400, 120, 320, 40 };
            Rectangle rec_city  = { 400, 200, 320, 40 };
            Rectangle rec_note  = { 400, 280, 320, 40 };

            // 2. Klikanie w 6 pól (Zmiana Focusu)
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, rec_fname)) focus = FOCUS_FNAME;
                else if (CheckCollisionPointRec(mouse, rec_lname)) focus = FOCUS_LNAME;
                else if (CheckCollisionPointRec(mouse, rec_phone)) focus = FOCUS_PHONE;
                else if (CheckCollisionPointRec(mouse, rec_email)) focus = FOCUS_EMAIL;
                else if (CheckCollisionPointRec(mouse, rec_city))  focus = FOCUS_CITY;
                else if (CheckCollisionPointRec(mouse, rec_note))  focus = FOCUS_NOTE;
                else focus = FOCUS_NONE;
            }

            // 3. Rysowanie pól
            // --- LEWA KOLUMNA ---
            DrawTextEx(myFont, "Imie:", (Vector2){40, 95}, 20, 1, textColor);
            DrawRectangleRec(rec_fname, cardColor);
            DrawRectangleLinesEx(rec_fname, 2, (focus == FOCUS_FNAME) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_fname, (Vector2){rec_fname.x + 10, rec_fname.y + 10}, 20, 1, textColor);

            DrawTextEx(myFont, "Nazwisko:", (Vector2){40, 175}, 20, 1, textColor);
            DrawRectangleRec(rec_lname, cardColor);
            DrawRectangleLinesEx(rec_lname, 2, (focus == FOCUS_LNAME) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_lname, (Vector2){rec_lname.x + 10, rec_lname.y + 10}, 20, 1, textColor);

            DrawTextEx(myFont, "Telefon:", (Vector2){40, 255}, 20, 1, textColor);
            DrawRectangleRec(rec_phone, cardColor);
            DrawRectangleLinesEx(rec_phone, 2, (focus == FOCUS_PHONE) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_phone, (Vector2){rec_phone.x + 10, rec_phone.y + 10}, 20, 1, textColor);

            // --- PRAWA KOLUMNA ---
            DrawTextEx(myFont, "Email:", (Vector2){400, 95}, 20, 1, textColor);
            DrawRectangleRec(rec_email, cardColor);
            DrawRectangleLinesEx(rec_email, 2, (focus == FOCUS_EMAIL) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_email, (Vector2){rec_email.x + 10, rec_email.y + 10}, 20, 1, textColor);

            DrawTextEx(myFont, "Miasto / Adres:", (Vector2){400, 175}, 20, 1, textColor);
            DrawRectangleRec(rec_city, cardColor);
            DrawRectangleLinesEx(rec_city, 2, (focus == FOCUS_CITY) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_city, (Vector2){rec_city.x + 10, rec_city.y + 10}, 20, 1, textColor);

            DrawTextEx(myFont, "Notatka:", (Vector2){400, 255}, 20, 1, textColor);
            DrawRectangleRec(rec_note, cardColor);
            DrawRectangleLinesEx(rec_note, 2, (focus == FOCUS_NOTE) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_note, (Vector2){rec_note.x + 10, rec_note.y + 10}, 20, 1, textColor);


            // Komunikat o błędzie na dole
            if (error_msg[0] != '\0') {
                DrawTextEx(myFont, error_msg, (Vector2){40, 360}, 18, 1, RED);
            }

            // 4. Przyciski: Anuluj i Zapisz (przesunięte w dół)
            Rectangle btnCancel = { 40, 420, 120, 40 };
            DrawRectangleRounded(btnCancel, 0.2f, 10, GRAY);
            DrawTextEx(myFont, "Anuluj", (Vector2){btnCancel.x + 25, btnCancel.y + 10}, 20, 1, WHITE);

            Rectangle btnSave = { 180, 420, 120, 40 };
            DrawRectangleRounded(btnSave, 0.2f, 10, primaryColor);
            DrawTextEx(myFont, "Zapisz", (Vector2){btnSave.x + 25, btnSave.y + 10}, 20, 1, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                
                if (CheckCollisionPointRec(mouse, btnCancel)) {
                    stan = EKRAN_LISTY; 
                }
               else if (CheckCollisionPointRec(mouse, btnSave)) {
                    int valid = 1; // Flaga: 1 = wszystko super, 0 = jest błąd
                    error_msg[0] = '\0'; // Czyścimy poprzednie błędy

                    // --- 1. Walidacja: Imię i Nazwisko (Wymagane, tylko litery/spacje/myślniki) ---
                    if (strlen(buf_fname) == 0 || strlen(buf_lname) == 0) {
                        valid = 0; strcpy(error_msg, "Blad: Imie i Nazwisko sa wymagane!");
                    } else {
                        // Sprawdzamy imię
                        for (int j = 0; buf_fname[j] != '\0'; j++) {
                            char c = buf_fname[j];
                            // Dopuszczamy litery (isalpha), spacje, myślniki i apostrofy (częste w obcych imionach)
                            if (!isalpha(c) && c != '-' && c != ' ' && c != '\'') {
                                valid = 0; strcpy(error_msg, "Blad: Imie moze zawierac tylko litery!"); break;
                            }
                        }
                        // Sprawdzamy nazwisko (tylko jeśli imię przeszło)
                        for (int j = 0; buf_lname[j] != '\0' && valid; j++) {
                            char c = buf_lname[j];
                            if (!isalpha(c) && c != '-' && c != ' ' && c != '\'') {
                                valid = 0; strcpy(error_msg, "Blad: Nazwisko moze zawierac tylko litery!"); break;
                            }
                        }
                    }

                    // --- 2. Walidacja: Telefon (Format 9 cyfr LUB +48 i 9 cyfr) ---
                    if (valid) {
                        int len = strlen(buf_phone);
                        int phone_ok = 1;
                        
                        if (len == 9) {
                            // Musi być 9 samych cyfr
                            for (int j = 0; j < 9; j++) {
                                if (!isdigit(buf_phone[j])) phone_ok = 0;
                            }
                        } else if (len == 12 && strncmp(buf_phone, "+48", 3) == 0) {
                            // Musi zaczynać się od +48, a potem 9 cyfr
                            for (int j = 3; j < 12; j++) {
                                if (!isdigit(buf_phone[j])) phone_ok = 0;
                            }
                        } else {
                            phone_ok = 0; // Zła długość
                        }

                        if (!phone_ok) {
                            valid = 0; strcpy(error_msg, "Blad: Telefon: 9 cyfr lub +48XXXXXXXXX");
                        }
                    }

                    // --- 3. Walidacja: Email (Musi mieć @ i domenę, np. .com) ---
                    // Sprawdzamy tylko, jeśli użytkownik w ogóle coś wpisał w to pole
                    if (valid && strlen(buf_email) > 0) {
                        char *at_ptr = strchr(buf_email, '@'); // Szukamy znaku @
                        
                        if (at_ptr == NULL || at_ptr == buf_email) {
                            valid = 0; strcpy(error_msg, "Blad: Email musi zawierac poprawny znak @");
                        } else {
                            char *dot_ptr = strchr(at_ptr, '.'); // Kropka musi być PO znaku @
                            
                            // Kropka nie może być od razu po @ (np. uzytkownik@.com) ani na samym końcu (np. user@mail.)
                            if (dot_ptr == NULL || dot_ptr == at_ptr + 1 || *(dot_ptr + 1) == '\0') {
                                valid = 0; strcpy(error_msg, "Blad: Email musi posiadac domene (np. .pl)");
                            }
                        }
                    }

                    // --- JEŚLI WSZYSTKO PRZESZŁO POMYŚLNIE ---
                    if (valid) {
                        int result = 0;
                        
                        if (stan == EKRAN_DODAWANIA) {
                            result = contact_add_gui(my_book, buf_fname, buf_lname, buf_phone, buf_email, buf_city, buf_note);
                        } else if (stan == EKRAN_EDYCJI) {
                            result = contact_edit_gui(my_book, edytowany_indeks, buf_fname, buf_lname, buf_phone, buf_email, buf_city, buf_note);
                        }
                        
                        if (result == 1) {
                            contact_sort_gui(my_book);
                            contact_save_gui(my_book, "src/contacts.csv"); 
                            
                            // Czyszczenie buforów
                            buf_fname[0] = '\0'; buf_lname[0] = '\0'; buf_phone[0] = '\0';
                            buf_email[0] = '\0'; buf_city[0]  = '\0'; buf_note[0]  = '\0';
                            error_msg[0] = '\0';
                            
                            stan = EKRAN_LISTY; 
                        } else {
                            strcpy(error_msg, "Blad: Nie udalo sie zapisac kontaktu do pamieci.");
                        }
                    }
                }
            }
        }

        EndDrawing();
    }
    contact_save_gui(my_book, "src/contacts.csv");
    UnloadFont(myFont);
    CloseWindow();
}