CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

EXECUTABLES = Grayscale_Blur_Serial

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)
