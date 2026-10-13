CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = minidb

SRC = $(wildcard src/*.c)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)