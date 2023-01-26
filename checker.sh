#! /bin/bash

rm ./output/*
make clean -C ./Grayscale_Blur_Serial/
make clean -C ./Grayscale_Blur_OpenMP/
make clean -C ./Grayscale_Blur_MPI/
make clean -C ./Grayscale_Blur_Pthreads/
make clean -C ./Grayscale_Blur_Hibrid_MPI_OpenMP/
make clean -C ./Grayscale_Blur_Hibrid_MPI_Pthreads/
make -C ./Grayscale_Blur_Serial/
make -C ./Grayscale_Blur_OpenMP/
make -C ./Grayscale_Blur_MPI/
make -C ./Grayscale_Blur_Pthreads/
make -C ./Grayscale_Blur_Hibrid_MPI_OpenMP/
make -C ./Grayscale_Blur_Hibrid_MPI_Pthreads/

echo -e "\nRunning Grayscale & Gaussian Blur algorithms ..."

for i in {1..5}
do
    echo -e "\nRunning Test$i:"

    echo -n -e "\nTest$i secvential time ---------->"
    ./Grayscale_Blur_Serial/Grayscale_Blur_Serial ./input/test$i.bmp ./output/test$i-serial.bmp

    echo -n -e "\nTest$i OpenMP time -------------->"
    ./Grayscale_Blur_OpenMP/Grayscale_Blur_OpenMP ./input/test$i.bmp ./output/test$i-OpenMP.bmp

    echo -n -e "\nTest$i MPI time ----------------->"
    mpirun --oversubscribe -np 8 ./Grayscale_Blur_MPI/Grayscale_Blur_MPI ./input/test$i.bmp ./output/test$i-MPI.bmp

    echo -n -e "\nTest$i Pthreads time ------------>"
    ./Grayscale_Blur_Pthreads/Grayscale_Blur_Pthreads ./input/test$i.bmp ./output/test$i-pthreads.bmp 8

    echo -n -e "\nTest$i Hibrid MPI-OpenMP time --->"
    mpirun --oversubscribe -np 4 ./Grayscale_Blur_Hibrid_MPI_OpenMP/Grayscale_Blur_Hibrid_MPI_OpenMP ./input/test$i.bmp ./output/test$i-hibrid_MPI_OpenMP.bmp

    echo -n -e "\nTest$i Hibrid MPI-Pthreads time ->"
    mpirun --oversubscribe -np 4 ./Grayscale_Blur_Hibrid_MPI_Pthreads/Grayscale_Blur_Hibrid_MPI_Pthreads ./input/test$i.bmp ./output/test$i-hibrid_MPI_Pthreads.bmp

    echo ""
done

echo -e "\nChecking outputs for OpenMP..."
for i in {1..5}
do
    DIFF=$(diff ./output/test$i-serial.bmp ./output/test$i-OpenMP.bmp) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for MPI..."
for i in {1..5}
do
    DIFF=$(diff ./output/test$i-serial.bmp ./output/test$i-MPI.bmp) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for Pthreads..."
for i in {1..5}
do
    DIFF=$(diff ./output/test$i-serial.bmp ./output/test$i-pthreads.bmp) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for Hibrid MPI-OpenMP..."
for i in {1..5}
do
    DIFF=$(diff ./output/test$i-serial.bmp ./output/test$i-hibrid_MPI_OpenMP.bmp) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done

echo -e "\nChecking outputs for Hibrid MPI-Pthreads..."
for i in {1..5}
do
    DIFF=$(diff ./output/test$i-serial.bmp ./output/test$i-hibrid_MPI_Pthreads.bmp) 
    if [ "$DIFF" != "" ] 
    then
        echo "Test$i failed"
    else
        echo "Test$i passed"
    fi
done