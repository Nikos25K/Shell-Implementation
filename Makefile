# Compiler
CC = gcc

# Directories
SRC = ./src
INCLUDE = ./include
BIN = ./bin
BUILD = ./build
COMMANDS_DIR := $(SRC)/commands
HELPING_DIR := $(SRC)/helping

# Compiler flags
CFLAGS = -pthread -Wall -g3 -std=c99 -I$(INCLUDE)

# Source files in src/commands
COMMAND_SRCS := $(wildcard $(COMMANDS_DIR)/*.c)
# Object files for commands
COMMAND_OBJS := $(patsubst $(COMMANDS_DIR)/%.c,$(BUILD)/%.o,$(COMMAND_SRCS))

# Source files in src/helping
HELPING_SRCS := $(wildcard $(HELPING_DIR)/*.c)
# Object files for helping
HELPING_OBJS := $(patsubst $(HELPING_DIR)/%.c,$(BUILD)/%.o,$(HELPING_SRCS))

# Source files
SRCS := $(wildcard $(SRC)/*.c)
# Object files
OBJS := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SRCS))

# Add command object files to the list of object files
OBJS += $(COMMAND_OBJS)
# Add helping object files to the list of object files
OBJS += $(HELPING_OBJS)

# Executable name
TARGET = $(BIN)/mysh

# Create directories if they don't exist
.PHONY: directories
directories:
	@mkdir -p $(BIN) $(BUILD)

# Rule to build object files from command source files
$(BUILD)/%.o: $(COMMANDS_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build object files from helping source files
$(BUILD)/%.o: $(HELPING_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build object files from source files
$(BUILD)/%.o: $(SRC)/%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Run the executable
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Run valgrind on the executable
.PHONY: valgrind
valgrind: $(TARGET)
	valgrind ./$(TARGET) --leak-check=full

# Clean rule
.PHONY: clean
clean:
	rm -f $(BUILD)/*.o $(TARGET)

# All rule
.PHONY: all
all: $(TARGET)

# Help rule
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all: Build all executables"
	@echo "  clean: Clean object files and executables"
	@echo "  help: Display this help message"
	@echo "  run: Run the executable"
	@echo "  valgrind: Run valgrind on the executable"