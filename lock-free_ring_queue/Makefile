
TOP = ./..
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
TARGET = main
SRC = $(wildcard *.c)
INC = -I $(TOP)/include
LINK = -lpthread

all:
	$(CC) $(CFLAGS) -o$(TARGET) $(SRC) $(INC) $(LINK)

test:
	./$(TARGET)

clean:
	rm $(TARGET)
