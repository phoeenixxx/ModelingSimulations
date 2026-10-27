/*
 * TASK PROBLEM STATEMENT:
 * -----------------------------------------------------------------------------
 * Problem 6:
 * Write the simulation division function that performs 630360016 mod (2^31 - 1)
 * operation based on the bitwise operations. (3 points)
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// Helper function to print 32-bit binary representation
inline void printBinary32(unsigned long long n) {
    string s = "";
    for (int i = 31; i >= 0; --i) {
        s += ((n >> i) & 1) ? "1" : "0";
    }
    cout << "0b" << s;
}

// Bitwise Modulo for Mersenne Prime M = 2^31 - 1
inline unsigned long long bitwiseModMersenne31(unsigned long long x) {
    const unsigned long long MASK_31 = (1ULL << 31) - 1; // 2147483647 (0x7FFFFFFF)
    
    // Fast Mersenne reduction: X mod (2^31 - 1) = (X >> 31) + (X & (2^31 - 1))
    while (x >= MASK_31) {
        x = (x >> 31) + (x & MASK_31);
    }
    return (x == MASK_31) ? 0 : x;
}

// Standard modulo for benchmark comparison
inline unsigned long long standardModMersenne31(unsigned long long x) {
    const unsigned long long MOD = (1ULL << 31) - 1;
    return x % MOD;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    unsigned long long X = 630360016ULL;
    unsigned long long M = (1ULL << 31) - 1; // 2^31 - 1 = 2147483647

    unsigned long long res_bitwise = bitwiseModMersenne31(X);
    unsigned long long res_standard = standardModMersenne31(X);

    cout << "=== INPUT DATA ===\n";
    cout << "Value (X):    " << X << " | Hex: 0x" << hex << X << dec << "\n";
    cout << "Modulo (2^31-1): " << M << " | Hex: 0x" << hex << M << dec << "\n\n";

    cout << "=== BITWISE SIMULATION BREAKDOWN ===\n";
    cout << "High bits (X >> 31):         " << (X >> 31) << "\n";
    cout << "Low bits  (X & (2^31 - 1)):  " << (X & M) << "\n";
    cout << "Bitwise Result:              " << res_bitwise << " | Binary: ";
    printBinary32(res_bitwise);
    cout << "\nStandard Modulo Result (%):  " << res_standard << "\n\n";

    // Performance Benchmark (100,000,000 iterations)
    const int iterations = 100000000;
    volatile unsigned long long dummy1 = X;
    volatile unsigned long long dummy2 = X;

    auto start_bw = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        // Larger test value to force bitwise reduction in benchmark
        dummy1 = bitwiseModMersenne31(630360016ULL + i); 
    }
    auto end_bw = chrono::high_resolution_clock::now();
    chrono::duration<double> bw_time = end_bw - start_bw;

    auto start_std = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy2 = standardModMersenne31(630360016ULL + i);
    }
    auto end_std = chrono::high_resolution_clock::now();
    chrono::duration<double> std_time = end_std - start_std;

    cout << "=== BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Bitwise Mersenne Mod Time: " << bw_time.count() << " seconds\n";
    cout << "Standard Modulo (%) Time:   " << std_time.count() << " seconds\n";

    double speedup = std_time.count() / bw_time.count();
    cout << "\nIn C++, Bitwise Mersenne reduction is ~" << fixed << setprecision(2) 
         << speedup << "x FASTER!\n";

    return 0;
}
