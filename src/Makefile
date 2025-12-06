# Library configuration
LIB_TETRIS := brick_game/tetris/tetris.a
OBJ_TETRIS := brick_game/tetris/tetris.o
SRC_TETRIS := brick_game/tetris/tetris.c
HDR_TETRIS := brick_game/tetris/tetris.h
HDR_API := brick_game/brick_game.h

# Compiler and flags
CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -pedantic
LDFLAGS := -lncurses
AR := ar
ARFLAGS := rcs

# Directories
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/1/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

# Source files
SOURCES := \
	main.c \
	gui/cli/cli.c \
	brick_game/tetris/tetris.c

HEADERS := \
	gui/cli/cli.h \
	brick_game/brick_game.h \
	brick_game/tetris/tetris.h

# Object files (исправлено: правильные пути в build/)
OBJ_FILES := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/gui/cli/cli.o

# Executable
TARGET := tetris
TARGET_PATH := $(BIN_DIR)/$(TARGET)

# Main target: all
# Builds static library AND executable

.PHONY: all
all: $(LIB_TETRIS) $(TARGET_PATH)

# ============================================================================
# TODO: Сборка статической библиотеки tetris.a
# ============================================================================
# Статическая библиотека создаётся командой ar (архиватор)
# ar rcs - создаёт архив из объектных файлов
# r = replace (заменить/добавить)
# c = create (создать если не существует)
# s = index (создать индекс символов)

$(LIB_TETRIS): $(OBJ_TETRIS)
	$(AR) $(ARFLAGS) $@ $^

# Компиляция tetris.c в tetris.o
# $(OBJ_TETRIS): $(SRC_TETRIS) $(HDR_TETRIS) $(HDR_API)
# 	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_TETRIS): $(SRC_TETRIS)
	$(CC) $(CFLAGS) -c $(LIB_TETRIS) -o $(OBJ_TETRIS)

# ============================================================================
# Executable target (исправлено!)
# ============================================================================
# Было: TARGET_PATH: - неправильно (нет $)
# Стало: $(TARGET_PATH): - правильно

$(TARGET_PATH): $(LIB_TETRIS) $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIB_TETRIS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# Directory creation
# ============================================================================

$(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(GCOV_DIR):
	@mkdir -p $@

.PHONY: rebuild
rebuild: clean all
