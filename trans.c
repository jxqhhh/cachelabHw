/*
 * Name: Xinqi Jin
 * loginID: 2016010524
 */
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

char test_desc[]="Test"
void test(int M,int N,int A[N][M],int B[M][N]){
// Copy:
    for ( i = 0; i < 48; i ++ ) {
        for ( j = 0; j < 6; j ++ ) {
            a1 = A[i][8*j+0];
            a2 = A[i][8*j+1];
            a3 = A[i][8*j+2];
            a4 = A[i][8*j+3];
            a5 = A[i][8*j+4];
            a6 = A[i][8*j+5];
            a7 = A[i][8*j+6];
            a8 = A[i][8*j+7];
            B[8*j+0][i]= a1;
            B[8*j+1][i] = a2;
            B[8*j+2][i] = a3;
            B[8*j+3][i] = a4;
            B[8*j+4][i] = a5;
            B[8*j+5][i] = a6;
            B[8*j+6][i] = a7;
            B[8*j+7][i]= a8;
        }
    }

    // Transpose in-place:
    for ( i = 0; i < 48; i += 8) {
        for ( j = 0; j < 48; j += 8 ) {


            // non-diagonal 8*8 submatrix:S
            if( i < j ) {
                for ( k = i; k < i + 8; k ++ ) {
                    for ( h = j; h < j + 8; h ++ ) {
                        a1 = B[h][k];
                        a2 = B[k][h];
                        B[k][h] = a1;
                        B[h][k] = a2;
                    }
                }
            }
        }
    }
}

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i,j,k,h;
    int a1,a2,a3,a4,a5,a6,a7,a8;
    if ( M == 32 ) { // Case1: M=32, N=32
        for ( i = 0; i < 32; i += 8) {
            for ( j = 0; j < 32; j += 8 ) {
                // diagonal 8*8 submatirx:
                if ( i == j ) {
                    // copy the diagonal submatrix:
                    for ( k = i; k < i + 8; k ++ ) {
                        a1 = A[k][j];
                        a2 = A[k][j+1];
                        a3 = A[k][j+2];
                        a4 = A[k][j+3];
                        a5 = A[k][j+4];
                        a6 = A[k][j+5];
                        a7 = A[k][j+6];
                        a8 = A[k][j+7];
                        B[k][j] = a1;
                        B[k][j+1] = a2;
                        B[k][j+2] = a3;
                        B[k][j+3] = a4;
                        B[k][j+4] = a5;
                        B[k][j+5] = a6;
                        B[k][j+6] = a7;
                        B[k][j+7] = a8;
                    }

                    // transpose in-place:
                    for( k = i; k < i + 8 ; k ++ ){
                        for( h = k + 1 ; h < i + 8; h ++ ){
                            a1 = B[k][h];
                            a2 = B[h][k];
                            B[k][h] = a2;
                            B[h][k] = a1;
                        }
                    }
                    continue;
                }

                // non-diagonal 8*8 submatrix:S
                for ( k = i; k < i + 8; k ++ ) {
                    for ( h = j; h < j + 8; h ++ ) {
                        B[h][k] = A[k][h];
                    }
                }
            }
        }
    } else if ( M == 64 ) { // Case2: M=64, N=64
        for ( i = 0; i < 64; i += 8 ) {
            for ( j = 0; j < 64; j += 8 ) {

                // Step 1:
                for ( k = 0; k < 4; k ++ ) {
                    a1 = A[i+k][j];
                    a2 = A[i+k][j+1];
                    a3 = A[i+k][j+2];
                    a4 = A[i+k][j+3];
                    a5 = A[i+k][j+4];
                    a6 = A[i+k][j+5];
                    a7 = A[i+k][j+6];
                    a8 = A[i+k][j+7];
                    B[j][i+k] = a1;
                    B[j+1][i+k] = a2;
                    B[j+2][i+k] = a3;
                    B[j+3][i+k] = a4;
                    B[j][i+k+4] = a5;
                    B[j+1][i+k+4] = a6;
                    B[j+2][i+k+4] = a7;
                    B[j+3][i+k+4] = a8;
                }

                // Step 2:
                for ( k = 0; k < 4; k ++ ) {
                    a1 = A[i+4][j+k];
                    a2 = A[i+5][j+k];
                    a3 = A[i+6][j+k];
                    a4 = A[i+7][j+k];
                    a5 = B[j+k][i+4];
                    a6 = B[j+k][i+5];
                    a7 = B[j+k][i+6];
                    a8 = B[j+k][i+7];
                    B[j+k][i+4] = a1;
                    B[j+k][i+5] = a2;
                    B[j+k][i+6] = a3;
                    B[j+k][i+7] = a4;
                    B[j+k+4][i] = a5;
                    B[j+k+4][i+1] = a6;
                    B[j+k+4][i+2] = a7;
                    B[j+k+4][i+3] = a8;
                }
                
                // Step 3:
                for ( k = i + 4; k < i + 8; k ++ ) {
                    for ( h = j + 4; h < j + 8; h ++ ){
                        B[h][k]=A[k][h];
                    }
                }

            }
        }
    } else if ( M == 61 ) { // Case3: M=61, N=67
        for ( i = 0; i < 80; i += 16 ) {
            for ( j = 0; j < 64; j += 16 ) {
                for ( k = i; ( k < i + 16 ) && ( k < 67 ); k ++ ) {
                    for ( h = j; ( h < j + 16 ) && ( h < 61 ); h ++ ) {
                        B[h][k]=A[k][h];
                    }
                }
            }
        }
    } else { // Case4: M=64, N=64

        // Copy:
        for ( i = 0; i < 48; i ++ ) {
            for ( j = 0; j < 6; j ++ ) {
                a1 = A[i][8*j+0];
                a2 = A[i][8*j+1];
                a3 = A[i][8*j+2];
                a4 = A[i][8*j+3];
                a5 = A[i][8*j+4];
                a6 = A[i][8*j+5];
                a7 = A[i][8*j+6];
                a8 = A[i][8*j+7];
                B[i][8*j+0] = a1;
                B[i][8*j+1] = a2;
                B[i][8*j+2] = a3;
                B[i][8*j+3] = a4;
                B[i][8*j+4] = a5;
                B[i][8*j+5] = a6;
                B[i][8*j+6] = a7;
                B[i][8*j+7] = a8;
            }
        }

        // Transpose in-place:
        for ( i = 0; i < 48; i += 8) {
            for ( j = 0; j < 48; j += 8 ) {

                // diagonal 8*8 submatirx:
                if ( i == j ) {
                    for( k = i; k < i + 8 ; k ++ ){
                        for( h = k + 1 ; h < i + 8; h ++ ){
                            a1 = B[k][h];
                            a2 = B[h][k];
                            B[k][h] = a2;
                            B[h][k] = a1;
                        }
                    }
                    continue;
                }

                // non-diagonal 8*8 submatrix:S
                if( i < j ) {
                    for ( k = i; k < i + 8; k ++ ) {
                        for ( h = j; h < j + 8; h ++ ) {
                            a1 = B[h][k];
                            a2 = B[k][h];
                            B[k][h] = a1;
                            B[h][k] = a2;
                        }
                    }
                }
            }
        }
    }
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

