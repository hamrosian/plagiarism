# Define the compiler
CC = gcc

VERSION = gnu11

# Define the compiler flags
CFLAGS = -Wall -Wextra

# Define the linker flags
LDFLAGS = -lz -lm

# Source and build directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Create object files for each .c file
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# The name of the executable
TARGET = $(BIN_DIR)/plagiarism

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) -std=$(VERSION) $(OBJS) -o $@ $(LDFLAGS)

# Compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -std=$(VERSION) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
