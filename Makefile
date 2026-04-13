# Makefile for Bank Management System (Cross-Platform fallback)
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC = src/main.c src/ui.c src/auth.c src/storage.c src/transaction.c src/user.c
OBJ = $(SRC:.c=.o)
TARGET = bms

# Detect OS
ifdef OS
   RM = del /Q
   DEL_OBJ = del /Q src\*.o
   TARGET_EXT = .exe
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      DEL_OBJ = rm -f src/*.o
      TARGET_EXT =
   endif
endif

all: $(TARGET)$(TARGET_EXT)

$(TARGET)$(TARGET_EXT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(DEL_OBJ)
	$(RM) $(TARGET)$(TARGET_EXT)

.PHONY: all clean
