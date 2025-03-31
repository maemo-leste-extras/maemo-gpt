CC = gcc
CFLAGS = -Wall `pkg-config --cflags gtk+-2.0`
LDFLAGS = `pkg-config --libs gtk+-2.0` -lcurl -lglib-2.0 -ljansson

SRC = main.c gtk2ui.c file_handling.c parsing.c on_click.c
OBJ = $(SRC:.c=.o)

all: maemo-gpt

maemo-gpt: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.o: .c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(OBJ) maemo-gpt
