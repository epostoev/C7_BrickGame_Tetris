# Compiler and flags
CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -pedantic
LDFLAGS := -lncurses
TESTFLAGS := -lcheck -lm -lpthread -lrt -lsubunit
GCOVFLAGS := -fprofile-arcs -ftest-coverage
CLANG_FORMAT ?= clang-format

# Directories
SRC_DIR := .
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
TEST_DIR := tests
DOC_DIR := documentation
DIST_DIR := dist
INSTALL_DIR ?= /usr/local
PREFIX ?= $(INSTALL_DIR)

# Source files
SOURCES := \
	main.c \
	gui/cli/cli.c \
	brick_game/tetris/tetris.c \

HEADERS := \
	gui/cli/cli.h \
	brick_game/brick_game.h \
	brick_game/tetris/tetris.h \

# Object files
OBJECTS := $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

# Target executable
TARGET := tetris
TARGET_PATH := $(BIN_DIR)/$(TARGET)

.PHONY: all
all: $(TARGET_PATH)

# Build the main executable
$(TARGET_PATH): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✓ Build complete: $(TARGET_PATH)"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#TODO что делает @mkdir -p $(dir $@)
#TODO что делает $(CC) $(CFLAGS) -c $< -o $@
#TODO сделать цель на сборку статической библиотеки tetris.a
#TODO Эта библиотека должна быть использована в 43 строчке в цели ALL



# Create necessary directories
$(OBJ_DIR) $(BIN_DIR) $(DIST_DIR) $(GCOV_DIR) $(DOC_DIR):
	@mkdir -p $@

clean:
	rm a.out
format:
	@$(CLANG_FORMAT) -i $(SOURCES)
#TODO добавить цель на создание файла .clang-format (копирование из материалс)

format-check:
	@$(CLANG_FORMAT) -n $(SOURCES)

test:
#TODO Сделать цель на покрытие и на тест

gcov_report: