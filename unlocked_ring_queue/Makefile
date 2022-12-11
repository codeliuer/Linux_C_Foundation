
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
TARGET = main
LINK =
OBJ = main.c ring.c ring.h

all:
	$(CC) $(CFLAGS) -o$(TARGET) $(OBJ) $(LINK)

test:
	./$(TARGET)

clean:
	rm $(TARGET)
