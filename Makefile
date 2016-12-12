# Exercise configuration

STATIC_OBJS =
EXTENDIBLE_OBJS =

BIN_STATIC = build/static.out
BIN_EXTENDIBLE = build/extendible.out

# Flags passed to preprocessor and compiler

CPPFLAGS += -iquoteinclude
CFLAGS += -std=c99 -pedantic-errors
ifeq ($(CC), clang)
	CFLAGS += -Weverything -g
else
	CFLAGS += -Wall -Wextra -g3
endif
LDFLAGS += -Llib -lblock$(shell getconf LONG_BIT)

# Build targets

.PHONY = all clean
all : $(BIN_STATIC) $(BIN_EXTENDIBLE)
clean :
	$(RM) -r build/*

# Exercise builds

$(BIN_STATIC) : src/main.c $(STATIC_OBJS)
	$(CC) $(CPPFLAGS) -D STATIC -D EXTENDIBLE=0 $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_EXTENDIBLE) : src/main.c $(EXTENDIBLE_OBJS)
	$(CC) $(CPPFLAGS) -D EXTENDIBLE -D STATIC=0 $(CFLAGS) $^ -o $@ $(LDFLAGS)
