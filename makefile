CC = g++
CFLAGS = -O3 -std=c++11
LDFLAGS = -fopenmp

SRC = magic_matrix.cpp
TARGET = xmmGPU
GPU_TARGET = magic_matrix_gpu.cu

all:
	make xmmGPU

$(TARGET): $(SRC)
	$(CC) -o xmm $(SRC) $(LDFLAGS)
	ncc $(LDFLAGS) -mp=gpu $(GPU_TARGET) -o $(TARGET)

$(GPU_TARGET): $(SRC)
	mmgpu -o $(GPU_TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f xmm
