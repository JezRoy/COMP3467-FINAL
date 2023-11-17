#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <unordered_set>

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

// NOTE TO SELF: vvvvvvv
// COMPILE THIS CODE: nvc++ -fopenmp -mp=gpu magic_matrix.cpp -o xmm
// RUN THIS CODE: ./mm data_sets/pattern3x3.dat data_sets/modifier3x3.dat
// OR THIS CODE: ./mmpgu data_sets/pattern20x20.dat data_sets/modifier20x20.dat

double omp_get_wtime(void);

void generateMagicSquare(int** pattern, int** modifier, int** magicSquare, int N, int M)
{
    double start;
    double end;
    start = omp_get_wtime();
    #pragma omp target map(to:i, j, N) map(tofrom:modifier[0:N][0:N]) distribute parallel for
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                modifier[i][j] *= M;
            }
        }
    }
    #pragma omp target map(to:i, j, M) map(tofrom:magicSquare[0:N][0:N]) distribute parallel for
    {
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
    end = omp_get_wtime();
    printf("Function 'generateMagicSquare' took %f seconds to complete\n", end - start);
}

// computes sum of elements in a row
int sumRow( int** matrix, int row, int N)
{
    int sum = 0;
    #pragma omp teams distribute parallel for reduction(+:sum)
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
    #pragma omp teams distribute parallel for reduction(+:sum)
    for (int i = 0; i < N; i++)
    {
        sum += matrix[i][col];
    }
    return sum;
}

// checks if all elements in an array are equal
bool allEqual( int arr[], int N)
{
    double start;
    double end;
    start = omp_get_wtime();
    for (int i = 0; i < N; i++){
        if (arr[0] != arr[i])
	{
            end = omp_get_wtime();
            printf("Function 'allEqual' took %f seconds to complete\n", end - start);
            return false;
        }
    }
    end = omp_get_wtime();
    printf("Function 'allEqual' took %f seconds to complete\n", end - start);
    return true;
}

// Takes 50 seconds
bool isPairwiseDistinctOLD( int** matrix, int N) {
    double start;
    double end;
    start = omp_get_wtime();
    int duplicatesFound = 0; 
    #pragma omp target teams map(to: matrix[0:N][0:N]) map(tofrom: duplicatesFound) parallel for collapse(2) reduction(+:duplicatesFound)
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int currentElement = matrix[i][j];
                for (int row = 0; row < N; row++) {
                    for (int col = 0; col < N; col++) {
                        if (row != i || col != j) {
                            int otherElement = matrix[row][col];
                            if (currentElement == otherElement) {
                                duplicatesFound = duplicatesFound + 1;
                            }
                        }
                    }
                }
            }
        }
    }
    end = omp_get_wtime();
    printf("Function 'isPairwiseDistinct' took %f seconds to complete\n", end - start);
    if (duplicatesFound > 0) {
        printf("found duplicate\n");
        return false;
    } else {
        printf("no duplicate found\n");
        return true;
    }
}

// improved function leveraging hashing to achieve better performance
// Takes 0.05 - 0.06 seconds
bool isPairwiseDistinct(int** matrix, int N) {
    double start, end;
    start = omp_get_wtime();
    // Create an unordered set to store unique elements encountered
    std::unordered_set<int> elements;
    bool foundDuplicate = false; // Flag to indicate if a duplicate is found

    #pragma omp target map(matrix[0:N][0:N]) parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int currentElement = matrix[i][j];

            // Use hashing to check uniqueness
            #pragma omp critical
            {
                if (elements.find(currentElement) != elements.end()) {
                    // Set the flag and exit the loop if a duplicate is found
                    foundDuplicate = true;
                } else {
                    elements.insert(currentElement); // Add the element to the set
                }
            }
        }
    }

    end = omp_get_wtime();
    printf("Function 'isPairwiseDistinct' took %f seconds to complete\n", end - start);

    if (foundDuplicate) {
        printf("Duplicate elements found\n");
        return false; // Return false if duplicates are found
    } else {
        printf("No duplicate elements found\n");
        return true; // Return true if no duplicates are found
    }
}

// checks if matrix is a magic square
bool isMagicSquare(int** matrix, int N) {
    double start, end;
    start = omp_get_wtime();
    int row_sums[N];
    int col_sums[N];
    int main_diag_sum = 0;
    int anti_diag_sum = 0;

    #pragma omp target teams distribute parallel for map(to: matrix[0:N][0:N]) map(tofrom: row_sums[0:N], col_sums[0:N], main_diag_sum, anti_diag_sum)
    for (int i = 0; i < N; i++) {
        row_sums[i] = sumRow(matrix, i, N);
        col_sums[i] = sumColumn(matrix, i, N);
        main_diag_sum += matrix[i][i];
        anti_diag_sum += matrix[i][N - 1 - i];
    }

    bool isMagic = true;
    if (!allEqual(row_sums, N) || !allEqual(col_sums, N) || main_diag_sum != row_sums[0] ||
        anti_diag_sum != row_sums[0] || !isPairwiseDistinct(matrix, N)) {
        isMagic = false;
    }

    end = omp_get_wtime();
    printf("Function 'isMagicSquare' took %f seconds to complete\n", end - start);

    return isMagic;
}

int main(int argc, char *argv[])
{
    double start;
    double end;
    start = omp_get_wtime();
    #pragma omp parallel()

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
    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
	    pattern[i] = new int[N];
	    modifier[i] = new int[N];
        for (int j = 0; j < N; j++) {
            if (fscanf(pattern_file, "%d", &pattern[i][j]) != 1) {
                printf("Error reading matrix values pattern.\n");
                fclose(pattern_file);
                end = omp_get_wtime();
                printf("Function 'main' took %f seconds to complete\n", end - start);
                return 1;
            }
            if (fscanf(modifier_file, "%d", &modifier[i][j]) != 1) {
                printf("Error reading matrix values modifier.\n");
                fclose(modifier_file);
                end = omp_get_wtime();
                printf("Function 'main' took %f seconds to complete\n", end - start);
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


    end = omp_get_wtime();
    printf("Function 'main' took %f seconds to complete\n", end - start);
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