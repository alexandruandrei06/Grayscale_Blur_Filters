# Grayscale Filter

In digital photography, computer-generated imagery, and colorimetry, a grayscale image is one in which the value of each pixel is a single sample representing only an amount of light; that is, it carries only intensity information.

More about Grayscale Filter: https://en.wikipedia.org/wiki/Grayscale

# Gaussian Blur Filter

In image processing, a Gaussian blur (also known as Gaussian smoothing) is the result of blurring an image by a Gaussian function.

More about Gaussian Blur Filter: https://en.wikipedia.org/wiki/Gaussian_blur

# Description

The project aims to parallelize the calculations for the application of a Grayscale filter and a Gaussian Blur filter, starting from the sequential implementation, with the aim of observing the differences between the different parallel programming methods and their performances.

-   Pthread
-   MPI
-   OpenMP
-   Hibrid MPI-Pthreads
-   Hibrid MPI-OpenMP

# Input file format

The input is a photo in format .bmp

# Output file format

The result is written as a picture in .bmp format

# MPI installation

```bash
sudo apt install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
```

# Usage

To compile, run and verify the correctness of the algorithms, you can run the 'check.sh'

```bash
./check.sh
```

How to run specific implementation:

-   Serial

```bash
./Grayscale_Blur_Serial input_file output_file
```

-   Pthreads

```bash
./Grayscale_Blur_Pthreads input_file output_file threads_no
```

-   MPI

```bash
mpirun --oversubscribe -np process_no ./Grayscale_Blur_MPI input_file output_file
```

-   OpenMP

```bash
./Grayscale_Blur_OpenMP input_file output_file
```

-   MPI-Pthreads

```bash
mpirun --oversubscribe -np process_no ./Grayscale_Blur_Hibrid_MPI_Pthreads input_file output_file
```

-   MPI-OpenMP

```bash
mpirun --oversubscribe -np process_no ./Grayscale_Blur_Hibrid_MPI_OpenMP input_file output_file
```
