CC = g++
CFLAGS = -O3 -std=c++11
LDFLAGS = -fopenmp

SRC = magic_matrix.cpp
TARGET = xmmGPU
GPU_TARGET = magic_matrix_gpu.cu

all: $(TARGET) $(GPU_TARGET)

$(TARGET): $(SRC)
    $(CC) $(CFLAGS) -o xmm $(SRC) $(LDFLAGS)
	$(CC) $(CFLAGS) -o $(TARGET) $(GPU_TARGET) $(LDFLAGS)

$(GPU_TARGET): $(SRC)
	nvcc -o $(GPU_TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(GPU_TARGET)
	rm -f xmm
