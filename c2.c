#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float dpunroll(long N, float *pA, float *pB){
    float R = 0.0;
    int j;
    for (j=0; j<N; j+=4){
        R += pA[j]*pB[j] + pA[j+1]*pB[j+1] + pA[j+2]*pB[j+2] + pA[j+3]*pB[j+3];
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

    float result = 0.0;

    for (int i=0; i<rep; i++){
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        result = dpunroll(N, pA, pB);
        clock_gettime(CLOCK_MONOTONIC, &end);
        times[i] = ((double)(end.tv_sec - start.tv_sec)*1000000) + ((double)(end.tv_nsec - start.tv_nsec)/1000);
    }
    
    printf("Result: %.1f\n", result);

    double total_time = 0.0;

    for (int i=rep/2; i<rep; i++){
        total_time += times[i];
    }

    double avg_time = total_time / (rep/2);
    double bandwidth = (8.0*(N/4)*sizeof(float)) / ((avg_time / 1000000) * (1024 * 1024 * 1024));
    double throughput = (8.0*(N/4)) / (avg_time*1000);

    printf("N: %ld <T>: %.6f sec B: %.3f GB/sec F: %.3f FLOP/sec (%.3f GFLOP/sec)\n", N, avg_time/1000000, bandwidth, throughput * 1000000000, throughput);

    free(pA);
    free(pB);
    free(times);
    
    return 0;
}
