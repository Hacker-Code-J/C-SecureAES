# Compiler settings - Can be customized.
CC = gcc
CFLAGS = -I./include -O2 -Wall -Wextra -pedantic

# Source files
SRC = ./src/aes_key_expansion.c ./src/utils.c
OBJ = $(SRC:./src/%.c=./obj/%.o)

# Test program
TEST_SRC = ./examples/key_expansion_test.c
TEST_OUT = a.out

# Compile and build
all: $(TEST_OUT)

$(TEST_OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(TEST_OUT) $(TEST_SRC) $(OBJ)

# Compile source files into object files
./obj/%.o: ./src/%.c
	mkdir -p ./obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build
clean:
	rm -rf ./obj $(TEST_OUT)

rebuild: clean all

# Check Memory Leak
leak:
	valgrind --leak-check=full --show-leak-kinds=all ./a.out

test-key:
	@echo "Testing ..."
	./a.out > round_keys.txt
	mv round_keys.txt tests/
	(cd tests && python3 key_expansion.py)

# Phony targets
.PHONY: all clean


# # Makefile for AES Cryptographic Library

# CC = gcc
# CFLAGS = -Wall -Wextra -O2
# INCLUDES = -Iinclude
# SRC_DIR = src
# OBJ_DIR = obj
# BIN_DIR = bin

# # Define the source and object files
# SRCS = $(SRC_DIR)/aes.c $(SRC_DIR)/aes_key_expansion.c $(SRC_DIR)/aes_modes.c $(SRC_DIR)/utils.c
# OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# DEPS = $(OBJS:.o=.d)

# # Target binary
# TARGET = $(BIN_DIR)/aes_library

# # Default target
# all: $(TARGET)

# # Include dependencies
# -include $(DEPS)

# # Rule to create the target directory
# $(BIN_DIR):
# 	mkdir -p $(BIN_DIR)

# # Rule to create the object directory
# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# # Rule to create the binary
# $(TARGET): $(OBJS) | $(BIN_DIR)
# 	$(CC) $(CFLAGS) -o $@ $^

# # Rule to create object files
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
# 	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# # Clean target
# clean:
# 	rm -rf $(OBJ_DIR) $(BIN_DIR)

# # Phony targets
# .PHONY: all clean