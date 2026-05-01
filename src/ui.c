#include <stdio.h>
#include <string.h> // Potrzebne do obsługi tekstów (strlen)
#include "ui.h"
#include "raylib.h" 
#include "structs.h"
#include "logic_gui.h" // Podpinamy nową logikę!

// 1. Definicja stanów aplikacji i skupienia (focusu)
typedef enum { EKRAN_LISTY, EKRAN_DODAWANIA } StanAplikacji;
typedef enum { FOCUS_NONE, FOCUS_FNAME, FOCUS_LNAME, FOCUS_PHONE } FocusState;

void start_gui(struct ContactArray *my_book) {
    InitWindow(800, 600, "Contact Manager");
    SetTargetFPS(60);

    Font myFont = LoadFont("assets/Inter_18pt-Bold.ttf");

    Color bgColor = (Color){ 245, 245, 250, 255 };       
    Color cardColor = WHITE;                             
    Color textColor = (Color){ 40, 40, 40, 255 };        
    Color primaryColor = (Color){ 74, 144, 226, 255 };   

    // Zmienne do zarządzania stanem
    StanAplikacji stan = EKRAN_LISTY;
    FocusState focus = FOCUS_NONE;

    // Bufory przechowujące to, co użytkownik wpisze z klawiatury
    char buf_fname[50] = {0};
    char buf_lname[50] = {0};
    char buf_phone[20] = {0};
    char error_msg[100] = {0}; // Komunikat o ewentualnym błędzie

    while (!WindowShouldClose()) {
        
        // --- LOGIKA AKTUALIZACJI (Wpisywanie z klawiatury) ---
        if (stan == EKRAN_DODAWANIA) {
            char *active_buf = NULL;
            size_t max_len = 0;

            // Ustalamy, który bufor jest aktywny
            if (focus == FOCUS_FNAME) { active_buf = buf_fname; max_len = 49; }
            else if (focus == FOCUS_LNAME) { active_buf = buf_lname; max_len = 49; }
            else if (focus == FOCUS_PHONE) { active_buf = buf_phone; max_len = 19; }

            if (active_buf != NULL) {
                // Łapanie wpisywanych znaków
                int key = GetCharPressed();
                while (key > 0) {
                    // Dopuszczamy standardowe znaki drukowane
                    if ((key >= 32) && (key <= 125) && (strlen(active_buf) < max_len)) {
                        int len = strlen(active_buf);
                        active_buf[len] = (char)key;
                        active_buf[len + 1] = '\0';
                    }
                    key = GetCharPressed(); // Pobieramy następny klawisz z kolejki
                }

                // Obsługa Backspace
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    int len = strlen(active_buf);
                    if (len > 0) active_buf[len - 1] = '\0';
                }
            }
        }

        // --- RYSOWANIE ---
        BeginDrawing();
        ClearBackground(bgColor);

        if (stan == EKRAN_LISTY) {
            // --- EKRAN 1: LISTA KONTAKTÓW ---
            DrawTextEx(myFont, "Twoje Kontakty", (Vector2){40, 30}, 36, 1, primaryColor);

            // Przycisk "Dodaj kontakt"
            Rectangle btnAdd = { 560, 30, 200, 40 };
            DrawRectangleRounded(btnAdd, 0.2f, 10, primaryColor);
            DrawTextEx(myFont, "+ Dodaj Kontakt", (Vector2){btnAdd.x + 15, btnAdd.y + 10}, 20, 1, WHITE);

            // Przejście do ekranu dodawania
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btnAdd)) {
                stan = EKRAN_DODAWANIA;
                error_msg[0] = '\0'; // Czyścimy błędy przy wejściu
            }

            int startY = 100;       
            int cardHeight = 80;    
            int spacing = 15;       

            for (int i = 0; i < my_book->size; i++) {
                int currentY = startY + (i * (cardHeight + spacing));
                if (currentY > 600) break;

                Rectangle cardRec = {40, currentY, 720, cardHeight};
                DrawRectangleRounded(cardRec, 0.2f, 10, cardColor);
                DrawRectangleRoundedLines(cardRec, 0.2f, 10, LIGHTGRAY); 

                const char *fullName = TextFormat("%s %s", my_book->data[i].first_name, my_book->data[i].last_name);
                DrawTextEx(myFont, fullName, (Vector2){60, currentY + 15}, 24, 1, textColor);

                const char *phone = TextFormat("Tel: %s", my_book->data[i].phone);
                DrawTextEx(myFont, phone, (Vector2){60, currentY + 45}, 18, 1, GRAY);
            }
        } 
        else if (stan == EKRAN_DODAWANIA) {
            // --- EKRAN 2: DODAWANIE KONTAKTU ---
            DrawTextEx(myFont, "Dodaj Nowy Kontakt", (Vector2){40, 30}, 36, 1, primaryColor);

            // 1. Pola tekstowe - obszary (Rectangles)
            Rectangle rec_fname = { 40, 120, 300, 40 };
            Rectangle rec_lname = { 40, 190, 300, 40 };
            Rectangle rec_phone = { 40, 260, 300, 40 };

            // 2. Klikanie w pola (Focus)
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, rec_fname)) focus = FOCUS_FNAME;
                else if (CheckCollisionPointRec(mouse, rec_lname)) focus = FOCUS_LNAME;
                else if (CheckCollisionPointRec(mouse, rec_phone)) focus = FOCUS_PHONE;
                else focus = FOCUS_NONE;
            }

            // 3. Rysowanie pól
            // IMIĘ
            DrawTextEx(myFont, "Imie:", (Vector2){40, 95}, 20, 1, textColor);
            DrawRectangleRec(rec_fname, cardColor);
            DrawRectangleLinesEx(rec_fname, 2, (focus == FOCUS_FNAME) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_fname, (Vector2){rec_fname.x + 10, rec_fname.y + 10}, 20, 1, textColor);

            // NAZWISKO
            DrawTextEx(myFont, "Nazwisko:", (Vector2){40, 165}, 20, 1, textColor);
            DrawRectangleRec(rec_lname, cardColor);
            DrawRectangleLinesEx(rec_lname, 2, (focus == FOCUS_LNAME) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_lname, (Vector2){rec_lname.x + 10, rec_lname.y + 10}, 20, 1, textColor);

            // TELEFON
            DrawTextEx(myFont, "Telefon:", (Vector2){40, 235}, 20, 1, textColor);
            DrawRectangleRec(rec_phone, cardColor);
            DrawRectangleLinesEx(rec_phone, 2, (focus == FOCUS_PHONE) ? primaryColor : LIGHTGRAY);
            DrawTextEx(myFont, buf_phone, (Vector2){rec_phone.x + 10, rec_phone.y + 10}, 20, 1, textColor);

            // Komunikat o błędzie na czerwono
            if (error_msg[0] != '\0') {
                DrawTextEx(myFont, error_msg, (Vector2){40, 320}, 18, 1, RED);
            }

            // 4. Przyciski: Anuluj i Zapisz
            Rectangle btnCancel = { 40, 400, 120, 40 };
            DrawRectangleRounded(btnCancel, 0.2f, 10, GRAY);
            DrawTextEx(myFont, "Anuluj", (Vector2){btnCancel.x + 25, btnCancel.y + 10}, 20, 1, WHITE);

            Rectangle btnSave = { 180, 400, 120, 40 };
            DrawRectangleRounded(btnSave, 0.2f, 10, primaryColor);
            DrawTextEx(myFont, "Zapisz", (Vector2){btnSave.x + 25, btnSave.y + 10}, 20, 1, WHITE);

            // Obsługa kliknięć przycisków
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                
                if (CheckCollisionPointRec(mouse, btnCancel)) {
                    stan = EKRAN_LISTY; // Powrót bez zapisu
                }
                else if (CheckCollisionPointRec(mouse, btnSave)) {
                    // Walidacja - zabezpieczenie przed pustymi polami
                    if (strlen(buf_fname) == 0 || strlen(buf_phone) == 0) {
                        strcpy(error_msg, "Blad: Imie i Telefon sa wymagane!");
                    } else {
                        // PRZEKAZANIE DANYCH DO LOGIKI
                        // Dla uproszczenia email, address i note są wysyłane jako puste stringi ""
                        int result = contact_add_gui(my_book, buf_fname, buf_lname, buf_phone, "", "", "");
                        
                        if (result == 1) {
                            // Zapisane pomyślnie! Czyścimy bufory i wracamy do listy
                            buf_fname[0] = '\0';
                            buf_lname[0] = '\0';
                            buf_phone[0] = '\0';
                            error_msg[0] = '\0';
                            stan = EKRAN_LISTY;
                        } else {
                            strcpy(error_msg, "Blad: Nie udalo sie dodac kontaktu.");
                        }
                    }
                }
            }
        }

        EndDrawing();
    }

    UnloadFont(myFont);
    CloseWindow();
}