#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float dp(long N, float *pA, float *pB){
    float R = 0.0;
    int j;
    for (j=0; j<N; j++){
        R += pA[j]*pB[j];
    }
    return R;
}

int main(int argc, char *argv[]){

    long N = atoi(argv[1]);
    int rep = atoi(argv[2]);

    float *pA = (float*)malloc(N*sizeof(float));
    float *pB = (float*)malloc(N*sizeof(float));

    for (int i=0; i<N; i++){
        pA[i] = 1.0f;
        pB[i] = 1.0f;
    }

    double *times = (double*)malloc(rep*sizeof(double));

    struct timespec start, end;
    float result = 0.0;

    for (int i=0; i<rep; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = dp(N, pA, pB);
        clock_gettime(CLOCK_MONOTONIC, &end);
        times[i] = ((double)(end.tv_sec - start.tv_sec)*1000000) + ((double)(end.tv_nsec - start.tv_nsec)/1000);
    }

    printf("Result: %.1f\n", result);

    double total_time = 0.0;

    for (int i=rep/2; i<rep; i++){
        total_time += times[i];
    }

    double avg_time = total_time / (rep/2);
    double bandwidth = (2.0*N*sizeof(float)) / ((avg_time / 1000000) * (1024 * 1024 * 1024));
    double throughput = (2.0*N) / (avg_time*1000);

    printf("N: %ld <T>: %.6f sec B: %.3f GB/sec F: %.3f FLOP/sec (%.3f GFLOP/sec)\n", N, avg_time/1000000, bandwidth, throughput * 1000000000, throughput);

    free(pA);
    free(pB);
    free(times);
    
    return 0;
}
