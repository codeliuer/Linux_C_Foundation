
TOP = ./..
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
TARGET = main
OBJ = $(wildcard *.c)
INC = -I $(TOP)/include
LINK = 

all:
	$(CC) $(CFLAGS) -o$(TARGET) $(OBJ) $(INC) $(LINK)

test:
	./$(TARGET)

clean:
	rm $(TARGET)
