CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm  # Linker flags

# Source and object files
SRC = symnmf.c
OBJ = $(SRC:.c=.o)

# Executable name
EXE = symnmf

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies
symnmf.o: symnmf.c symnmf.h

clean:
	rm -f $(OBJ) $(EXE)

.PHONY: all clean
