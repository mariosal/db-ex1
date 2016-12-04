# Exercise configuration

OBJS =
LIBS = lib/block$(shell getconf LONG_BIT).o

BIN_STATIC = build/static.out
BIN_EXTENDIBLE = build/extendible.out

# Flags passed to preprocessor and compiler

CPPFLAGS += -iquoteinclude
CFLAGS += -std=c99 -pedantic-errors -g
ifeq ($(CC), clang)
	CFLAGS += -Weverything
else
	CFLAGS += -Wall -Wextra
endif

# Build targets

.PHONY = all clean
all : $(BIN_STATIC) $(BIN_EXTENDIBLE)
clean :
	$(RM) -r build/*

# Exercise builds

$(BIN_STATIC) : src/main.c $(OBJS) $(LIBS)
	$(CC) $(CPPFLAGS) -DSTATIC $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_EXTENDIBLE) : src/main.c $(OBJS) $(LIBS)
	$(CC) $(CPPFLAGS) -DEXTENDIBLE $(CFLAGS) $^ -o $@ $(LDFLAGS)
