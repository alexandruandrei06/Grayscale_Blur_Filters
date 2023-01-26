CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm -lpthread

EXECUTABLES = Grayscale_Blur_Pthreads

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)
