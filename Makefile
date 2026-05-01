FILES = src/main.c src/logic.c src/io.c src/ui.c src/validation.c src/logic_gui.c
CC = gcc
CFLAGS = -Wall -Wextra -I include -I raylib/include
LDFLAGS = -L raylib/lib
LDLIBS = -lraylib -lgdi32 -lwinmm

contact_manager.exe: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o contact_manager.exe $(LDFLAGS) $(LDLIBS)

clean:
	rm -f contact_manager.exe