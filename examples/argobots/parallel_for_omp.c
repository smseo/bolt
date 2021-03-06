/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */

/*
 * See LICENSE.txt in top-level directory.
 */

/* Pragma omp parallel for directive evaluation
 * Output: avg time
 */

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_ELEMS       5017600     /* 2GB */
#define EXT_LOOP_ELEM   2           /* 2GB */
#define IN_LOOP_ELEM    2           /* 2GB */
#define IN_LOOP_TH      1           /* 2GB */
#define NUM_REPS        1

/* Vector initialization */
void init(float *v, int n)
{
    int i = 0;
    for (i = 0; i < n; i++) {
        v[i] = i + 100.0f;
    }
}

/* Called after each test to be sure that the compiler does
   not avoid to execute the test */
void check(float *v, int n)
{
    int i = 0;
    for (i = 0; i < n; i++) {
        if (v[i] != (i + 100.0f) * 0.9f) {
            printf("v[%d]<=0.0f\n", i);
        }
    }
}

int main(int argc, char *argv[])
{
    int i, r, nthreads;
    double *time, avg_time = 0.0;
    float *v;

    #pragma omp parallel
    {
        #pragma omp master
        {
            nthreads = omp_get_num_threads();
        }
    }
    int n = (argc > 1) ? atoi(argv[1]) : NUM_ELEMS;
    int rep = (argc > 2) ? atoi(argv[2]) : 1;
    time = (double *)malloc(sizeof(double) * rep);
    v = (float *)malloc(sizeof(float) * n);
    init(v, n);
    for (r = 0; r < rep; r++) {
        time[r] = omp_get_wtime();
        #pragma omp parallel for
        for (i = 0; i < n; i++) {
            v[i] *= 0.9f;
        }
        time[r] = omp_get_wtime() - time[r];
        avg_time += time[r];
    }
    avg_time /= rep;
    check(v, n);
    printf("%d %d %f\n", nthreads, n, avg_time);

    free(time);
    free(v);

    return EXIT_SUCCESS;
}
