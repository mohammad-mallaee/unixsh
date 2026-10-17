#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

double get_rand_pt()
{
    return (2 * (double)rand() / RAND_MAX) - 1;
}

void *thread_fn(void *args)
{
    int *in = (int *)malloc(sizeof(int));
    *in = 0;
    int points = *(int *)args;
    for (int i = 0; i < points; i++)
    {
        double x = get_rand_pt();
        double y = get_rand_pt();
        int is_in = x * x + y * y <= 1;
        if (is_in)
        {
            (*in)++;
        }
    }
    return (void *)in;
}

int mont_carlo(char **command)
{
    int threads = 5;
    int points = 100000000;
    if (command[1] != NULL)
    {
        int a = sscanf(command[1], "%d", &threads);
        if (a <= 0)
        {
            printf("Invalid argument for number of threads.\n");
            return 1;
        }
    }
    if (command[2] != NULL)
    {
        int a = sscanf(command[2], "%d", &points);
        if (a <= 0)
        {
            printf("Invalid argument for number of points.\n");
            return 1;
        }
    }
    int points_per_thread = points / threads;
    int last_thread_points = points - ((threads - 1) * points_per_thread);
    pthread_t t[threads];
    for (int i = 0; i < threads - 1; i++)
    {
        pthread_create(&t[i], NULL, thread_fn, &points_per_thread);
    }
    pthread_create(&t[threads - 1], NULL, thread_fn, &last_thread_points);

    int *result[threads];
    for (int i = 0; i < threads; i++)
    {
        pthread_join(t[i], (void *)&result[i]);
    }
    int in = 0;
    for (int i = 0; i < threads; i++)
    {
        in += *result[i];
    }
    double approx = (double)in / points;
    printf("approx=%f, pi/4=%f\n", approx, M_PI_4);
    return 0;
}
