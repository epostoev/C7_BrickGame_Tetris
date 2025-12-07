# Library configuration
LIB_TETRIS := brick_game/tetris/tetris.a
OBJ_TETRIS := brick_game/tetris/tetris.o
SRC_TETRIS := brick_game/tetris/tetris.c
HDR_API := brick_game/brick_game.h

# Compiler and flags
CC := gcc
CFLAGS := -std=c11 -Wall -Werror -Wextra -pedantic
LDFLAGS := -lncurses
AR := ar
ARFLAGS := rcs

# Directories
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin


# Object files
OBJ_FILES := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/gui/cli/cli.o

# Executable
TARGET := tetris
TARGET_PATH := $(BIN_DIR)/$(TARGET)


# System directories

INSTALL_BINDIR := /usr/local/bin

# Main target: all
# Builds static library AND executable

.PHONY: all
all: $(LIB_TETRIS) $(TARGET_PATH)

# Сборка статической библиотеки tetris.a
# Статическая библиотека создаётся командой ar (архиватор)
# ar rcs - создаёт архив из объектных файлов
# r = replace (заменить/добавить)
# c = create (создать если не существует)
# s = index (создать индекс символов)

$(LIB_TETRIS): $(OBJ_TETRIS)
	$(AR) $(ARFLAGS) $(LIB_TETRIS) $(OBJ_TETRIS)

# Компиляция tetris.c в tetris.o
$(OBJ_TETRIS): $(SRC_TETRIS)
	$(CC) $(CFLAGS) -c $(SRC_TETRIS) -o $(OBJ_TETRIS)

# Executable target
# $@ - цель
$(TARGET_PATH): $(LIB_TETRIS) $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIB_TETRIS) -o $@ $(LDFLAGS)

# $(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $< - зависимость
# $@ - цель

$(OBJ_DIR)/main.o: main.c
	@mkdir -p $(OBJ_DIR)
	@echo "COMPILING: $@"
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/gui/cli/cli.o: gui/cli/cli.c
	mkdir -p $(OBJ_DIR)/gui/cli/
	$(CC) $(CFLAGS) -c $< -o $@

# Установка tetris в систему
install: all
	@echo "Installing $(TARGET)"
	install -d $(BIN_DIR)
	install -m 755 $(TARGET_PATH) $(INSTALL_BINDIR)/$(TARGET)




.PHONY: rebuild
rebuild: clean all
