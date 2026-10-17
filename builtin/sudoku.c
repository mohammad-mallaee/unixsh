#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

enum mode
{
    row,
    col,
    grid
};

struct ThreadArg
{
    int **matrix;
    int mode;
    int i;
    int j;
};

void printMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void *validator(void *args)
{
    struct ThreadArg *arg_struct = args;
    int *result = (int *)malloc(sizeof(int));
    if (arg_struct->mode == row)
    {
        for (int i = 0; i < 9; i++)
        {
            int v[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (int j = 0; j < 9; j++)
            {
                v[arg_struct->matrix[i][j] - 1] = 1;
            }
            for (int j = 0; j < 9; j++)
            {
                if (v[j] == 0)
                {
                    *result = 0;
                    return (void *)result;
                }
            }
        }
    }
    else if (arg_struct->mode == col)
    {
        for (int j = 0; j < 9; j++)
        {
            int v[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for (int i = 0; i < 9; i++)
            {
                v[arg_struct->matrix[i][j] - 1] = 1;
            }
            for (int i = 0; i < 9; i++)
            {
                if (v[i] == 0)
                {
                    *result = 0;
                    return (void *)result;
                }
            }
        }
    }
    else if (arg_struct->mode == grid)
    {
        int v[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int i = arg_struct->i; i < arg_struct->i + 3; i++)
        {
            for (int j = arg_struct->j; j < arg_struct->j + 3; j++)
            {
                v[arg_struct->matrix[i][j] - 1] = 1;
            }
        }
        for (int i = 0; i < 9; i++)
        {
            if (v[i] == 0)
            {
                *result = 0;
                return (void *)result;
            }
        }
    }
    else
    {
        *result = 0;
        return (void *)result;
    }
    *result = 1;
    return (void *)result;
}

int sudoku_validator(char **command)
{
    char *filepath = command[1];
    FILE *file;
    file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Error opening file !\n");
        return 1;
    }
    char *line;
    size_t linecap = 0;
    ssize_t linelen;
    int **matrix = malloc(10 * sizeof(int *));
    for (int i = 0; i < 9; i++)
    {
        matrix[i] = (int *)malloc(10 * sizeof(int));
        for (int j = 0; j < 9; j++)
        {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    int *result[11];
    struct ThreadArg args[11];
    pthread_t t[11];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int idx = 3 * i + j;
            args[idx].matrix = matrix;
            args[idx].mode = grid;
            args[idx].i = 3 * i;
            args[idx].j = 3 * j;
        }
    }

    // rows
    args[9].matrix = matrix;
    args[9].mode = row;
    // cols
    args[10].matrix = matrix;
    args[10].mode = col;

    for (int i = 0; i < 11; i++)
    {
        pthread_create(&t[i], NULL, validator, &args[i]);
    }

    int valid = 1;
    for (int i = 0; i < 11; i++)
    {
        pthread_join(t[i], (void *)&result[i]);
        if (*result[i] == 0)
        {
            valid = 0;
        }
    }

    if (valid)
    {
        printf("Hooray! Your sudoku was correct.\n");
    }
    else
    {
        printf("Sad :(. Your sudoku was incorrect.\n");
    }

    for (int i = 0; i < 9; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
    fclose(file);
    return 0;
}
