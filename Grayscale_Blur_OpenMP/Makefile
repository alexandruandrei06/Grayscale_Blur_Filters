CC = gcc
CFLAGS = -g -Wall -Wextra -fopenmp
LOADLIBES=-lm

EXECUTABLES = Grayscale_Blur_OpenMP

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)
