# Let's set up our compiler stuff
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Here are the directories we'll be using
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Our final executable
TARGET = $(BIN_DIR)/mlfq_simulator

# Just listing out our source and object files here
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/scheduler/scheduler.c \
          $(SRC_DIR)/utils/utils.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# The default build target
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

# A handy rule to just clean up the mess when we're done
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) results.csv

# Keep make from getting confused if files with these names exist

.PHONY: all directories run clean
