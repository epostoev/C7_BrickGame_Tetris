# Directories
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

# Executable
TARGET := tetris
TARGET_PATH := $(BIN_DIR)/$(TARGET)

# Версия проекта
VERSION := 1.0.0

# Имя архима
DIST_NAME := $(TARGET)-v$(VERSION)
DIST_DIR := dist
DIST_ARCHIVE := $(DIST_DIR)/$(DIST_NAME).tar.gz


# Library configuration
LIB_TETRIS := brick_game/tetris/tetris.a
OBJ_TETRIS := brick_game/tetris/tetris.o
SRC_TETRIS := brick_game/tetris/tetris.c
HDR_TETRIS := brick_game/tetris/tetris.h
SRC_CLI := gui/cli/cli.c
HDR_CLI := gui/cli/cli.h
HDR_API := brick_game/brick_game.h

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


# Файлы для включения
DIST_FILES := \
  $(SRC_TETRIS) \
  $(HDR_TETRIS) \
  $(SRC_CLI) \
  $(HDR_CLI) \
  $(HDR_API) \


# Object files
OBJ_FILES := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/gui/cli/cli.o


# System directories

INSTALL_BINDIR := $(HOME)/.local/bin

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
	@echo "Installing $(TARGET) ....."
	install -d $(INSTALL_BINDIR)
	install -m 755 $(TARGET_PATH) $(INSTALL_BINDIR)/$(TARGET)
	@echo "	Installation $(TARGET) complete!"
	@echo "	Installed $(TARGET) to $(INSTALL_BINDIR)/$(TARGET)"
	@echo "	Run with: $(TARGET)"


#  Uninstall tetris
uninstall:
	@echo "Uninstalling $(TARGET) ...."
	rm -rf $(INSTALL_BINDIR)/$(TARGET)
	@echo "	Uninstalled from $(INSTALL_BINDIR)/$(TARGET)"

.PHONY: clean
clean:
	@echo "Cleaning....."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(LIB_TETRIS) $(OBJ_TETRIS)
	@rm -rf $(TARGET)
	@echo "Cleaned complete!!!!"

# Создание архива
.PHONY: clean
dist: clean
	@tar -czvf ../$(DIST_NAME).tar.gz ./
	
format:
	cp ../materials/linters/.clang-format ./
	echo "Formating code...."
	clang-format -i $(DIST_FILES)

format-check:
	cp ../materials/linters/.clang-format ./
	echo "Check formating code...."
	clang-format -n $(DIST_FILES)

test: 


.PHONY: rebuild
rebuild: clean all
