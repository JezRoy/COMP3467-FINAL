#!/bin/bash
#SBATCH --job-name=magic_matrix_parallel
#SBATCH --output=magic_matrix_output.txt
#SBATCH --gres=gpu
#SBATCH --partition=ug-gpu-small
#SBATCH --nodes=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:30:00

# Load required modules
srun -n 1 -c 2 --gres=gpu:1g.10gb:1 --partition=ug-gpu-small --pty /bin/bash
module load nvidia-hpc
module load cuda
module load nvc++

# Compile executable
make clean
make mm

# Define paths
dir="data_sets"
EXECUTABLE="./mm"

# Execute for N = 3
echo "Running magic_matrix for N=3..."
$EXECUTABLE $dir/pattern3x3.dat $dir/modifier3x3.dat

# Execute for N = 10
echo "Running magic_matrix for N=10..."
$EXECUTABLE $dir/pattern10x10.dat $dir/modifier10x10.dat
