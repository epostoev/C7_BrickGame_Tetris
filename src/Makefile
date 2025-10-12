CLANG_FORMAT ?= clang-format

SOURCES := \
	main.c \
	gui/cli/cli.c \
	gui/cli/cli.h \
	brick_game/brick_game.h \
	brick_game/tetris/tetris.c \
	brick_game/tetris/tetris.h \




all:
	gcc main.c gui/cli/cli.c brick_game/tetris/tetris.c -lncurses
clean:
	rm a.out
format:
	@$(CLANG_FORMAT) -i $(SOURCES)
format-check:
	@$(CLANG_FORMAT) -n $(SOURCES)