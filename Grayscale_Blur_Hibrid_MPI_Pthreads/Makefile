CC = mpicc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm -lpthread

EXECUTABLES = Grayscale_Blur_Hibrid_MPI_Pthreads

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): util.o

util.o: util.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES)

run:
	mpirun --oversubscribe -np 4 ./Grayscale_Blur_Hibrid_MPI_Pthreads ../input/test2.bmp ../output/test2-hibrid_MPI_Pthreads.bmp
