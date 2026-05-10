#include <stdio.h>
#include <string.h> 
#include "ui.h"
#include "raylib.h" 
#include "structs.h"
#include "logic_gui.h" 

// Rozbudowane stany focusu (dla wszystkich 6 pól)
typedef enum { EKRAN_LISTY, EKRAN_DODAWANIA } StanAplikacji;
typedef enum { FOCUS_NONE, FOCUS_FNAME, FOCUS_LNAME, FOCUS_PHONE, FOCUS_EMAIL, FOCUS_CITY, FOCUS_NOTE } FocusState;

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

    while (!WindowShouldClose()) {
        
        // --- LOGIKA WPISYWANIA Z KLAWIATURY ---
        if (stan == EKRAN_DODAWANIA) {
            char *active_buf = NULL;
            size_t max_len = 0; // Używamy size_t, żeby nie było warningu od kompilatora

            if (focus == FOCUS_FNAME) { active_buf = buf_fname; max_len = 49; }
            else if (focus == FOCUS_LNAME) { active_buf = buf_lname; max_len = 49; }
            else if (focus == FOCUS_PHONE) { active_buf = buf_phone; max_len = 19; }
            else if (focus == FOCUS_EMAIL) { active_buf = buf_email; max_len = 49; }
            else if (focus == FOCUS_CITY)  { active_buf = buf_city; max_len = 49; }
            else if (focus == FOCUS_NOTE)  { active_buf = buf_note; max_len = 99; }

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
        }

        // --- RYSOWANIE ---
        BeginDrawing();
        ClearBackground(bgColor);

        if (stan == EKRAN_LISTY) {
            DrawTextEx(myFont, "Twoje Kontakty", (Vector2){40, 30}, 36, 1, primaryColor);

            Rectangle btnAdd = { 560, 30, 200, 40 };
            DrawRectangleRounded(btnAdd, 0.2f, 10, primaryColor);
            DrawTextEx(myFont, "+ Dodaj Kontakt", (Vector2){btnAdd.x + 15, btnAdd.y + 10}, 20, 1, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnAdd)) {
                stan = EKRAN_DODAWANIA;
                error_msg[0] = '\0'; 
            }

            int startY = 90;       
            int cardHeight = 110;  // Zwiększone ze 80 na 110, by pomieścić więcej tekstu
            int spacing = 15;      

            // --- OBSŁUGA SCROLLOWANIA ---
            scroll_y += GetMouseWheelMove() * 30.0f; // Szybkość scrolla
            
            // Ograniczenia: blokada przewijania w kosmos (góra/dół)
            if (scroll_y > 0) scroll_y = 0; 
            int total_height = my_book->size * (cardHeight + spacing);
            int max_scroll = 600 - startY - total_height;
            if (max_scroll > 0) max_scroll = 0; // Jeśli mało kontaktów, brak scrolla
            if (scroll_y < max_scroll) scroll_y = max_scroll;

            // Uruchamiamy maskowanie: kafelki będą rysowane TYLKO w tym oknie
            // Dzięki temu podczas scrollowania znikają pod górnym nagłówkiem
            BeginScissorMode(0, startY, 800, 600 - startY);

            for (int i = 0; i < my_book->size; i++) {
                // Obliczamy pozycję z uwzględnieniem przewijania
                int currentY = startY + (int)scroll_y + (i * (cardHeight + spacing));
                
                // Optymalizacja: nie rysujemy kafelków, których i tak nie widać
                if (currentY + cardHeight < startY) continue; 
                if (currentY > 600) break;

                Rectangle cardRec = {40, currentY, 720, cardHeight};
                DrawRectangleRounded(cardRec, 0.2f, 10, cardColor);
                DrawRectangleRoundedLines(cardRec, 0.2f, 10, LIGHTGRAY); 

                // --- STARE DANE (Lewa strona) ---
                const char *fullName = TextFormat("%s %s", my_book->data[i].first_name, my_book->data[i].last_name);
                DrawTextEx(myFont, fullName, (Vector2){60, currentY + 15}, 24, 1, textColor);

                const char *phone = TextFormat("Tel: %s", my_book->data[i].phone);
                DrawTextEx(myFont, phone, (Vector2){60, currentY + 45}, 18, 1, GRAY);

                // --- NOWE DANE (Prawa strona i dół) ---
                const char *email = TextFormat("Email: %s", my_book->data[i].email);
                DrawTextEx(myFont, email, (Vector2){400, currentY + 15}, 18, 1, GRAY);

                const char *city = TextFormat("Miasto: %s", my_book->data[i].address);
                DrawTextEx(myFont, city, (Vector2){400, currentY + 45}, 18, 1, GRAY);

                const char *note = TextFormat("Notatka: %s", my_book->data[i].note);
                DrawTextEx(myFont, note, (Vector2){60, currentY + 75}, 16, 1, DARKGRAY);
            }

            EndScissorMode(); // Wyłączenie maskowania
        } 
        else if (stan == EKRAN_DODAWANIA) {
            DrawTextEx(myFont, "Dodaj Nowy Kontakt", (Vector2){40, 30}, 36, 1, primaryColor);

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
                    // Walidacja - zabezpieczenie przed pustym imieniem i telefonem
                    if (strlen(buf_fname) == 0 || strlen(buf_phone) == 0) {
                        strcpy(error_msg, "Blad: Imie i Telefon sa wymagane!");
                    }else if (CheckCollisionPointRec(mouse, btnSave)) {
                    // Walidacja...
                    if (strlen(buf_fname) == 0 || strlen(buf_phone) == 0) {
                        strcpy(error_msg, "Blad: Imie i Telefon sa wymagane!");
                    } else {
                        // Dodanie do RAM...
                        int result = contact_add_gui(my_book, buf_fname, buf_lname, buf_phone, buf_email, buf_city, buf_note);
                        
                        if (result == 1) {
                            // !!! AUTOMATYCZNY ZAPIS DO PLIKU !!!
                            contact_save_gui(my_book, "src/contacts.csv");

                            // Czyszczenie...
                            buf_fname[0] = '\0';
                            buf_lname[0] = '\0'; // (i reszta buforów...)
                            error_msg[0] = '\0';
                            stan = EKRAN_LISTY;
                        } else {
                            strcpy(error_msg, "Blad: Nie udalo sie dodac kontaktu.");
                        }
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