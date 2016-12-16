# Exercise configuration

OBJS = build/record.o \
       build/hash.o
EXOBJS = build/record.o

BIN = build/hash.out
EXBIN = build/exhash.out

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
all : $(BIN) $(EXBIN)
clean :
	$(RM) -r build/*

# Exercise builds

build/record.o : src/record.c include/record.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/hash.o : src/hash.c include/hash.h include/record.h include/BF.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/main.o : src/main.c include/BF.h include/hash.h include/record.h
	$(CC) $(CPPFLAGS) -D STATIC $(CFLAGS) -c $< -o $@

build/exmain.o : src/main.c include/BF.h include/exhash.h include/record.h
	$(CC) $(CPPFLAGS) -D EXTENDIBLE $(CFLAGS) -c $< -o $@

$(BIN) : build/main.o $(OBJS)
	$(CC) $(CPPFLAGS) -D STATIC $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(EXBIN) : build/exmain.o $(EXOBJS)
	$(CC) $(CPPFLAGS) -D EXTENDIBLE $(CFLAGS) $^ -o $@ $(LDFLAGS)
