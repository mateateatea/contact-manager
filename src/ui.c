#include <stdio.h>
#include "ui.h"
#include "raylib.h" 
#include "structs.h"

void start_gui(struct ContactArray *my_book) {
    InitWindow(800, 600, "Contact Manager");
    SetTargetFPS(60);

    // Załadowanie Twojej czcionki (upewnij się, że nazwa pliku się zgadza!)
    Font myFont = LoadFont("assets/Inter_18pt-Bold.ttf");

    // Definiujemy paletę kolorów
    Color bgColor = (Color){ 245, 245, 250, 255 };       // Jasnoszare tło aplikacji
    Color cardColor = WHITE;                             // Białe kafelki kontaktów
    Color textColor = (Color){ 40, 40, 40, 255 };        // Ciemnoszary tekst
    Color primaryColor = (Color){ 74, 144, 226, 255 };   // Niebieski akcent na nagłówek

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(bgColor);
            
            // 1. Główny nagłówek aplikacji
            DrawTextEx(myFont, "Twoje Kontakty", (Vector2){40, 30}, 36, 1, primaryColor);

            // 2. Parametry kafelków
            int startY = 100;       // Od jakiej wysokości (Y) zaczynamy rysować
            int cardHeight = 80;    // Wysokość pojedynczego kafelka
            int spacing = 15;       // Odstęp między kafelkami

            // 3. Pętla rysująca kontakty
            for (int i = 0; i < my_book->size; i++) {
                
                // Obliczamy pionową pozycję (Y) dla aktualnego kafelka
                int currentY = startY + (i * (cardHeight + spacing));

                // Zabezpieczenie: jeśli kafelek wychodzi poza dolną krawędź ekranu (600px), 
                // przerywamy rysowanie, żeby nie marnować zasobów komputera.
                if (currentY > 600) {
                    break;
                }

                // Rysujemy tło kafelka (prostokąt z zaokrąglonymi rogami)
                Rectangle cardRec = {40, currentY, 720, cardHeight};
                DrawRectangleRounded(cardRec, 0.2f, 10, cardColor);
                DrawRectangleRoundedLines(cardRec, 0.2f, 10, LIGHTGRAY); // Delikatna ramka

                // Przygotowujemy i rysujemy imię oraz nazwisko
                const char *fullName = TextFormat("%s %s", my_book->data[i].first_name, my_book->data[i].last_name);
                DrawTextEx(myFont, fullName, (Vector2){60, currentY + 15}, 24, 1, textColor);

                // Przygotowujemy i rysujemy numer telefonu pod spodem
                const char *phone = TextFormat("Tel: %s", my_book->data[i].phone);
                DrawTextEx(myFont, phone, (Vector2){60, currentY + 45}, 18, 1, GRAY);
            }
            
        EndDrawing();
    }

    UnloadFont(myFont);
    CloseWindow();
}