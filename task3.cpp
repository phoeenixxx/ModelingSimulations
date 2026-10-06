/*
 * PROBLEM STATEMENT:
 * Linear Congruential Generator (LCG):
 *   z_i = (1031 * z_{i-1} + 113) mod 2^10,  z_0 = 1103
 *
 * Mathematical Bitwise Optimization:
 *   1) 1031 = 1024 + 7 = 2^10 + 2^2 + 2^1 + 2^0
 *   2) Since we take mod 2^10 (1024), any multiple of 1024 is 0.
 *      Therefore, (z << 10) & 1023 == 0.
 *   3) Simplified LCG step:
 *      z_i = (((z_{i-1} << 2) + (z_{i-1} << 1) + z_{i-1}) + 113) & 1023
 */

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// Helper to print binary numbers cleanly
inline void printBinary10Bit(unsigned int n) {
    string s = "";
    for (int i = 9; i >= 0; --i) {
        s += ((n >> i) & 1) ? "1" : "0";
    }
    cout << "0b" << s;
}

// Ultra-optimized bitwise LCG step function
inline unsigned int nextLCG_Bitwise(unsigned int z) {
    // 7 * z + 113 mod 1024
    return (((z << 2) + (z << 1) + z) + 113) & 1023;
}

// Standard LCG step function for benchmark comparison
inline unsigned int nextLCG_Standard(unsigned int z) {
    return (1031 * z + 113) % 1024;
}

int main() {
    // Fast I/O configuration
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Initial seed: z0 = 1103
    unsigned int z_0 = 1103;
    unsigned int z_current = z_0 & 1023; // Normalize z0 to 10 bits (79)

    cout << "=== INITIAL STATE ===\n";
    cout << "z_0 (Original): " << z_0 << "\n";
    cout << "z_0 (Mod 1024): " << z_current << " | Binary: ";
    printBinary10Bit(z_current);
    cout << "\n\n";

    cout << "=== FIRST 10 GENERATED NUMBERS (STEP-BY-STEP) ===\n";
    cout << left << setw(8) << "Step" 
         << setw(12) << "Base 10" 
         << setw(16) << "Base 2 (10-bit)" << "\n";
    cout << "-------------------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        z_current = nextLCG_Bitwise(z_current);
        cout << left << "z_" << setw(5) << i 
             << setw(12) << z_current << "| ";
        printBinary10Bit(z_current);
        cout << "\n";
    }
    cout << "\n";

    // Performance Benchmark (100,000,000 iterations)
    const int iterations = 100000000;
    volatile unsigned int dummy1 = z_0 & 1023;
    volatile unsigned int dummy2 = z_0 & 1023;

    auto start_bw = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy1 = (((dummy1 << 2) + (dummy1 << 1) + dummy1) + 113) & 1023;
    }
    auto end_bw = chrono::high_resolution_clock::now();
    chrono::duration<double> bw_time = end_bw - start_bw;

    auto start_std = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy2 = (1031 * dummy2 + 113) % 1024;
    }
    auto end_std = chrono::high_resolution_clock::now();
    chrono::duration<double> std_time = end_std - start_std;

    cout << "=== BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Bitwise LCG Time:  " << bw_time.count() << " seconds\n";
    cout << "Standard LCG Time: " << std_time.count() << " seconds\n";

    double speedup = std_time.count() / bw_time.count();
    cout << "\nIn C++, Bitwise operations are ~" << fixed << setprecision(2) 
         << speedup << "x FASTER!\n";

    return 0;
}
