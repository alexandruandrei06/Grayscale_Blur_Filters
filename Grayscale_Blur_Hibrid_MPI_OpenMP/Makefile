CC = mpicc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm -fopenmp

EXECUTABLES = Grayscale_Blur_Hibrid_MPI_OpenMP

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)

run:
	mpirun --oversubscribe -np 4 Grayscale_Blur_Hibrid_MPI_OpenMP ../input/test10.bmp ../output/test10-MPI.bmp
