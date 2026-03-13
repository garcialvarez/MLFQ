CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/mlfq_simulator

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/scheduler/scheduler.c \
          $(SRC_DIR)/utils/utils.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: directories $(TARGET)

directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/scheduler
	@mkdir -p $(OBJ_DIR)/utils

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) results.csv

.PHONY: all directories run clean
