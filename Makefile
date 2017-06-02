#This makefile is really temporary. I wil eventually make disc a proper library that can be installed, but for now it compiles as a program

COMPILENAMENIX = disc
COMPILENAMEWIN = disc

SOURCES = $(wildcard src/*.c)

NIXCC = gcc
WINCC = i686-w64-mingw32-gcc

C_OBJ = $(SOURCES:.o)

LIBSNIX = -lm -lcrypto -lssl -levent
LIBSWIN = -lm -mwindows -lcrypto -lssl -levent

COMPILE_FLAGS = -g -Wall -o

linux:
	$(NIXCC) $(COMPILE_FLAGS) $(COMPILENAMENIX) $(SOURCES) $(LIBSNIX)

windows:
	$(WINCC) $(COMPILE_FLAGS) $(COMPILENAMEWIN) $(SOURCES) $(LIBSWIN)

run:
	./$(COMPILENAMENIX)

clean:
	rm -f *.o
