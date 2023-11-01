#include <stdio.h>
#include <stdbool.h>

// The generateMagicSquare() function is supposed to generate a large matrix square from two smaller ones.
//
// Input: 
// 1. A square matrix "pattern" of size N*N
// 2. A square matrix "modifier" of size N*N
//
// Output:
// A square matrix "magicSquare" of size M = (N*N) * (N*N)
//
// Algorithm:
// 1. Multiply each element of the "modifier" with M.
// 2. Copy the "pattern" matrix into each N*N square tile of the "magicSquare" matrix
//    such that the resulting matrix consists of N*N "pattern" blocks.
// 3. For all pattern blocks:
//      Multiply all elements in the "pattern" block[i][j] with the element at modifier[i][j]. 
//

void generateMagicSquare(int** pattern, int** modifier, int** magicSquare, int N, int M)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
		    modifier[i][j] *= M;
	    }
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int patternRow = i % N;
            int patternCol = j % N;
            magicSquare[i][j] = pattern[patternRow][patternCol];
	        magicSquare[i][j] += modifier[i/N][j/N];
        }
    }
}

// computes sum of elements in a row
int sumRow( int** matrix, int row, int N)
{
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += matrix[row][i];
    }
    return sum;
}

// computes sum of elements in a column
int sumColumn( int** matrix, int col, int N)
{
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += matrix[i][col];
    }
    return sum;
}

// checks if all elements in an array are equal
bool allEqual( int arr[], int N)
{
    for (int i = 0; i < N; i++){
        if (arr[0] != arr[i])
	{
            return false;
        }
    }
    return true;
}

bool isPairwiseDistinct( int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int currentElement = matrix[i][j];
            for (int row = 0; row < N; row++) {
                for (int col = 0; col < N; col++) {
                    if (row != i || col != j) {
                        int otherElement = matrix[row][col];
                        if (currentElement == otherElement) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// checks if matrix is a magic square
bool isMagicSquare(int** matrix, int N)
{
    int row_sums[N];
    int col_sums[N];
    int main_diag_sum = 0;
    int anti_diag_sum = 0;

    // compute row sums
    for (int i = 0; i < N; i++)
    {
        row_sums[i] = sumRow(matrix, i, N);
    }
    if (!allEqual(row_sums, N)) return false;

    int row_sum = row_sums[0];

    // compute column sums
    for (int i = 0; i < N; i++)
    {
        col_sums[i] = sumColumn(matrix, i, N);
    }
    if (!allEqual(col_sums, N)) return false;

    // compute sum of elements on main diagonal
    for (int i = 0; i < N; i++)
    {
        main_diag_sum += matrix[i][i];
    }
    if (main_diag_sum != row_sum) return false;

    // compute sum of elements on antidiagonal
    for (int i = 0; i < N; i++)
    {
        anti_diag_sum += matrix[i][N - 1 - i];
    }
    if (anti_diag_sum != row_sum) return false;
    
    if(isPairwiseDistinct(matrix, N))
	    return false;
    return true;
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Usage: %s <pattern_filename> <modifier_filename>\n", argv[0]);
        return 1;
    }

    FILE *pattern_file = fopen(argv[1], "r");
    FILE *modifier_file = fopen(argv[2], "r");

    if (pattern_file == NULL) {
        printf("Error opening the pattern_file.\n");
        return 1;
    }

    if (modifier_file == NULL) {
        printf("Error opening the modifier_file.\n");
        return 1;
    }

    int N;
    if (fscanf(pattern_file, "%d", &N) != 1) {
        printf("Error reading the size of the matrix.\n");
        fclose(pattern_file);
        fclose(modifier_file);
        return 1;
    }

    if (fscanf(modifier_file, "%d", &N) != 1) {
        printf("Error reading the size of the matrix.\n");
        fclose(modifier_file);
        return 1;
    }

    // dynamically allocate memory for input and output matrices
    int** pattern = new int*[N];
    int** modifier = new int*[N];
    int M = N*N;

    int** magicSquare = new int*[M];
    for (int i = 0; i < M; i++) {
	    magicSquare[i] = new int[M];
    }

    // read-in matrix data
    for (int i = 0; i < N; i++) {
	    pattern[i] = new int[N];
	    modifier[i] = new int[N];
        for (int j = 0; j < N; j++) {
            if (fscanf(pattern_file, "%d", &pattern[i][j]) != 1) {
                printf("Error reading matrix values pattern.\n");
                fclose(pattern_file);
                return 1;
            }
            if (fscanf(modifier_file, "%d", &modifier[i][j]) != 1) {
                printf("Error reading matrix values modifier.\n");
                fclose(modifier_file);
                return 1;
            }
        }
    }

    fclose(pattern_file);
    fclose(modifier_file);

    generateMagicSquare(pattern, modifier, magicSquare, N, M);

    bool is_magic_square = isMagicSquare(magicSquare, M);

    // Print first 3 and last 3 elements of generated and checked matrix 
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", magicSquare[i][0]);
    }

    printf("... \n . \n . \n . \n");

    for (int j = M-3; j < M; j++)
    {
        printf("%d ", magicSquare[M-1][j]);
    }

    printf("\n");
    if(is_magic_square) printf("Generated matrix is a magic square.\n");
    else                printf("Generated matrix is not a magic square.\n");


    // free dynamically allocated memory
    for (int i = 0; i < M; i++) {
        delete[] magicSquare[i];
    }
    delete[] magicSquare;

    for (int i = 0; i < N; i++) {
	    delete[] pattern[i];
	    delete[] modifier[i];
    }
    delete[] pattern;
    delete[] modifier;
}