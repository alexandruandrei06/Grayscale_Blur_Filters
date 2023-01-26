CC = mpicc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

EXECUTABLES = Grayscale_Blur_MPI

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)

run:
	mpirun --oversubscribe -np 8 Grayscale_Blur_MPI ../input/test2.bmp ../output/test2-MPI.bmp
