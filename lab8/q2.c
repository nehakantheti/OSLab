// CS22B1081
// Neha Kantheti
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

int M1[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}}, M2[3][3] = {{3, 3, 3}, {2, 2, 2}, {1, 1, 1}};
int A[3][3], S[3][3], M[3][3];

void *thread_func(void *arg)
{
    int threadId = (intptr_t)arg;
    if (threadId == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                A[i][j] = M1[i][j] + M2[i][j];
            }
        }
    }
    else if (threadId == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                S[i][j] = M1[i][j] - M2[i][j];
            }
        }
    }
    else if (threadId == 2)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                M[i][j] = 0;
                for (int k = 0; k < 3; k++)
                {
                    M[i][j] += M1[i][k] * M2[k][j];
                }
            }
        }
    }
}

void printMatrix(int M[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, thread_func, (void *)0);
    pthread_create(&t2, NULL, thread_func, (void *)1);
    pthread_create(&t3, NULL, thread_func, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\nMatrix 1:\n");
    printMatrix(M1);


    printf("Matrix 2:\n");
    printMatrix(M2);

    printf("Addition Matrix :\n");
    printMatrix(A);
    printf("Subtraction Matrix :\n");
    printMatrix(S);
    printf("Multiplication Matrix :\n");
    printMatrix(M);
    return 0;
}