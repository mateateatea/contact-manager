FILES = src/main.c src/logic.c src/io.c src/ui.c src/validation.c
CC = gcc
CFLAGS = -Wall -Wextra -I include

contact_manager.exe: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o contact_manager.exe

clean:
	rm -f contact_manager.exe