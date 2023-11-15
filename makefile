CC = g++
CFLAGS = -O3 -std=c++11 -fopenmp
LDFLAGS = -fopenmp

SRC = magic_matrix.cpp
TARGET = xmm
GPU_TARGET = magic_matrix_gpu.cu
GPU_OUT = xmmGPU

all:
	make $(TARGET) 
	make $(GPU_OUT)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDFLAGS)

$(GPU_OUT): $(GPU_TARGET)
	nvcc $(GPU_TARGET) -o $(GPU_OUT)

clean:
	rm -f $(TARGET)
	rm -f $(GPU_OUT)
