import sys
import numpy as np
import time

def dp(N, A, B):
    R = 0.0
    for j in range(0, N):
        R += A[j]*B[j]
    return R

if __name__=='__main__':
    N = int(sys.argv[1])
    rep = int(sys.argv[2])

    A = np.ones(N, dtype=np.float32)
    B = np.ones(N, dtype=np.float32)

    times = [0.0] * rep
    result = 0.0

    for i in range(0, rep):
        start = time.monotonic_ns()
        result = dp(N, A, B)
        end = time.monotonic_ns()
        times[i] = (end - start) / 1000
    
    print(f"Result: {result:.1f}")
    total_time = 0.0

    for i in range(rep//2, rep):
        total_time += times[i]

    avg_time = total_time / (rep/2)

    bandwidth = (2.0 * N * np.dtype(np.float32).itemsize) / ((avg_time / 1000000) * (1024*1024*1024));
    throughput = (2.0 * N) / (avg_time * 1000);

    print(f"N: {N} <T>: {avg_time/1000000:.6f} sec B: {bandwidth:.3f} GB/sec F: {throughput*1000000000:.3f} FLOP/sec ({throughput:.3f} GFLOP/sec)")

