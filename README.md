# HPC-Benchmarking
This repository contains benchmarking scripts for evaluating a kernel's performance on a high-performance computing server. It measures execution time, memory bandwidth, and compute FLOPS, providing insights into system efficiency and computational throughput. Ideal for performance tuning and system comparisons.

*Note: All problems are executed on NYU HPC.*

## Dot Product Benchmark (C)

A micro-benchmark that investigates the performance of computing the dot-product in C that takes two arrays of `float` (32 bit) as input. The dimension of the vector space and the number of repetitions for the measurement are command line arguments, i.e. a call `c1 1000 10` performs 10 measurements on a dot product with vectors of size 1000. (Note: initialized all fields in the input vectors to 1.0)

```C
float dp(long N, float *pA, float *pB) {
	float R = 0.0;
	int j;
	for (j=0; j<N; j++)
		R += pA[j]*pB[j];
	return R;
}

```

Compiled with `gcc -O3 -Wall -o c1 c1.c`

The code is executed on a node with Intel(R) Xeon(R) CPU E5-2690 v2 @3.00GHz or similar and enough RAM, to prevent the 300000000 size runs from being
killed by the system!

The execution time of the function is measured with `clock gettime(CLOCK MONOTONIC)`.

The time for N=1000000 and N=300000000 is measured. We perform 1000 repetitions for the small case and 20 repetitions for the large case, computing the appropriate mean for the execution time for the second half of the repetitions.

### Results

![c1 result](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/c1.png?raw=true)

For the small case of N = 1000000 we performed 1000 repetitions, giving us an average time of 0.001192
seconds (i.e. 1.192 milliseconds), 6.25 GB/s memory bandwidth, and 1677616809.904 FLOP/sec (i.e 1.678
GFLOP/sec) throughput.

For the large case of N = 300000000 we performed 20 repetitions, giving us an average time of 0.374445
seconds (i.e. 374.445 milliseconds), 5.969 GB/s memory bandwidth, and 1602372923.297 FLOP/sec (i.e
1.602 GFLOP/sec) throughput.

## Dot Product Benchmark (with Loop Unrolling)

The same microbenchmark is performed with

```C
float dpunroll(long N, float *pA, float *pB) {
	float R = 0.0;
	int j;
	for (j=0;j<N;j+=4)
		R += pA[j]*pB[j] + pA[j+1]*pB[j+1]
			  + pA[j+2]*pB[j+2] + pA[j+3]*pB[j+3];
	return R;
}
```

### Results

![c2 result](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/c2.png?raw=true)

For the small case of N = 1000000 we performed 1000 repetitions, giving us an average time of 0.000323
seconds (i.e. 0.323 milliseconds), 23.052 GB/s memory bandwidth, and 6187969205.472 FLOP/sec (i.e 6.188
GFLOP/sec) throughput.

For the large case of N = 300000000 we performed 20 repetitions, giving us an average time of 0.165701
seconds (i.e. 165.701 milliseconds), 13.489 GB/s memory bandwidth, and 3620978216.233 FLOP/sec (i.e
3.621 GFLOP/sec) throughput.

## Dot Product Benchmark (MKL CBLAS Library)

The same microbenchmark is performed with

```C
#include <mkl_cblas.h>

float bdp(long N, float *pA, float *pB) {
	float R = cblas_sdot(N, pA, 1, pB, 1);
	return R;
}
```

### Results

![c3 result](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/c3.png?raw=true)

For the small case of N = 1000000 we performed 1000 repetitions, giving us an average time of 0.000251
seconds (i.e. 0.251 milliseconds), 29.685 GB/s memory bandwidth, and 7968615766.177 FLOP/sec (i.e 7.969
GFLOP/sec) throughput.

For the large case of N = 300000000 we performed 20 repetitions, giving us an average time of 0.160735
seconds (i.e. 160.735 milliseconds), 13.906 GB/s memory bandwidth, and 3732909979.865 FLOP/sec (i.e
3.733 GFLOP/sec) throughput.

## Dot Product Benchmark (Python)

The same microbenchmark is performed in Python using NumPy arrays as input.

```Python
A = np.ones(N,dtype=np.float32)
B = np.ones(N,dtype=np.float32)

def dp(N,A,B):
	R = 0.0;
	for j in range(0,N):
		R += A[j]*B[j]
	return R
```

### Results

![c4 result](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/c4.png?raw=true)

For the small case of N = 1000000 we performed 1000 repetitions, giving us an average time of 0.194088
seconds (i.e. 194.088 milliseconds), 0.038 GB/s memory bandwidth, and 10304588.372 FLOP/sec (i.e 0.010
GFLOP/sec) throughput.

For the large case of N = 300000000 we performed 20 repetitions, giving us an average time of 55.424325
seconds, 0.040 GB/s memory bandwidth, and 10825571.581 FLOP/sec (i.e 0.011 GFLOP/sec) throughput.

## Dot Product Benchmark (NumPy Library)

The same measurements are performed using `numpy.dot()`.


```Python
A = np.ones(N,dtype=np.float32)
B = np.ones(N,dtype=np.float32)

result = np.dot(A, B)
```

### Result

![c5 result](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/c5.png?raw=true)

For the small case of N = 1000000 we performed 1000 repetitions, giving us an average time of 0.000253
seconds (i.e. 0.253 milliseconds), 29.443 GB/s memory bandwidth, and 7903422891.049 FLOP/sec (i.e 7.903
GFLOP/sec) throughput.

For the large case of N = 300000000 we performed 20 repetitions, giving us an average time of 0.153108
seconds (i.e. 153.108 milliseconds), 14.599 GB/s memory bandwidth, and 3918798395.568 FLOP/sec (i.e
3.919 GFLOP/sec) throughput.

## Discussion

### Consequence of only using the second half of the measurements for computation of the mean

By using only the second half of the measurements for computing the average time we reduce the influence
of any initial overheads or setup time, that could affect the first few measurements. A few examples of
initialization overheads would be memory allocation, caching, branch prediction optimizations, or even CPU
frequency adjustments. Discarding the first half of the measurements can help remove these initialization
artifacts, providing a more accurate reflection of the typical performance once the system is steady.

Another consequence of only using the second half of the measurements is that we are reducing our data size,
which may lead to less statistical significance. For instance, in the large case, where we are only running 20 repetitions, we only use the 10 latest measurements for calculating the average time. These fewer data points generally are less reliable at estimating the true mean and have increased variability in the average runtime. If the number of repetitions are increased, we might see a much more reliable estimate of the runtime.

### Roofline Model

![roofline model](https://github.com/rugvedmhatre/HPC-Benchmarking/blob/main/images/plot.png?raw=true)

The above figure shows the roofline model for our measurements. The Peak FLOPS are 200 GLOPS and the Memory Bandwidth is 30 GB/s. The DRAM and CPU cross at Arithmetic Intensity (AI) equal to 6.667 FLOP/Byte.
Any AI which is below 6.667 FLOP/Byte is memory bound and which is above 6.667 FLOP/Byte is compute
bound. For our 10 measurements, the AI is 0.25 FLOP/Byte, as all the programs use the same underlying
algorithm ($O(N)$ complexity).

### Difference in performance explained

Using the N = 300000000 simple loop as the baseline, the difference in performance for the 5 measurements in the C and Python variants can be explained as:

- Benchmark 1: This benchmark involves a simple algorithm in C, with a for loop multiplying each element of
the vector one by one and adding it to the result. Since this algorithm loads one value at a time, and
the result calculation for the current iteration relies on the result calculation of the previous iteration,
it is unable to extract instruction level parallelism. As a result, the throughput is low as compared to
C2, C3 or C5. Comparing it to C4, it has a higher throughput, as Python is an interpreted language
with Ahead-Of-Time compilation (which means that the compiler misses out on optimizations).

- Benchmark 2: This benchmark improves upon the algorithm in C1, by unrolling a part of calculations. In
this, rather than multiplying one value from both vectors at a time and adding it to the result, we
multiply 4 values and add them to the result. This results in less number of iterations, and thus, less
looping overhead. Also, by multiplying 4 elements in one iteration, the processor is able to extract
higher instruction level parallelism, as it can pipeline 4 different calculations, and thus generate higher
throughput. This code has a higher throughput than C1 and C4, but it is still lower than C3 and C5.

- Benchmark 3: This benchmark utilizes the dot product function from Intel’s Math Kernel Library for C. It is
able to extract a higher throughput than C1, C2 and C4. Reading about this library, I found out that
it is able to achieve higher throughput by using SIMD (Single Instruction/Multiple Data) or SIMD-like
instructions, and extract higher parallelism. However, the throughput is a bit less as compared to C5,
as it might be how the vectors are stored in memory.

- Benchmark 4: This program was the worst performing all the ones tested, as it relied completely on the Python
interpreter. With no optimizations in place, the program was really slow with an average runtime of
approximately 55 seconds and a throughput of 0.011 GFLOPS.

- Benchmark 5: This program performed the best, with the highest throughput (3.919 GFLOPS). Reading about
this implementation, I found out that it utilizes the same dot product function from Intel’s MKL for
C. However, it is able to achieve a bit better performance than C3, as it is storing the vectors in an
optimized way, and thus able to load vectors faster from the memory and acheive faster computation
speeds.

### Difference in dot product results explained

For the large case in C1, C2 and C4, the results are different from the analytically calculated ones. The
result should be 300,000,000.0 but it is 16,777,216.0 for C1 and C4 and 67,108,864.0 for C2. This is mainly
because of floating point errors which accumulate in calculating one-by-one. The results in C2 are still a bit
closer to the actual one, as the loop was unrolled, and the iterations were lesser, and thus the floating point
errors didn’t accumulate much.

C3 and C5 were exact to the analytically calculated ones, as they might be using higher precision or might
have optimizations to avoid the accumulation of floating point errors.

*Note: I was able to get an accurate result on C1 and C2 by using a double to store the result, rather than
using a float datatype.*
