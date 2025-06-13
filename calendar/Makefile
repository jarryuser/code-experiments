# Files
SRC_DIR = src
OBJ_DIR = BUILD
BIN = ${SRC_DIR}/calendar


SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


# Compilator and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Default point
all: $(BIN)


# Building binary
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@


# Building .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN)


run: all
	./$(SRC_DIR)/calendar
