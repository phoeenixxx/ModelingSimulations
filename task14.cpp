/*
 * TASK PROBLEM STATEMENT:
 * -----------------------------------------------------------------------------
 * Problem 14:
 * Write a function of Quadratic Congruential Generator:
 * 
 *     z_i = (11 * z_{i-1}^2 + 7 * z_{i-1} + 3) mod 16,   z_0 = 14
 * 
 * heavy moves operations on bits
 * -----------------------------------------------------------------------------
 * Bitwise Optimization Breakdown:
 *   - Modulo 16: x % 16 => x & 15 (0xF)
 *   - 11 * z^2  => (z^2 << 3) + (z^2 << 1) + z^2
 *   - 7 * z     => (z << 2) + (z << 1) + z
 */

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// Helper function to print 4-bit binary numbers
inline void printBinary4Bit(unsigned int n) {
    string s = "";
    for (int i = 3; i >= 0; --i) {
        s += ((n >> i) & 1) ? "1" : "0";
    }
    cout << "0b" << s;
}

// Ultra-optimized Bitwise Quadratic LCG step function
inline unsigned int nextQCG_Bitwise(unsigned int z) {
    unsigned int z_sq = z * z;
    // 11 * z_sq = (z_sq << 3) + (z_sq << 1) + z_sq
    unsigned int term1 = (z_sq << 3) + (z_sq << 1) + z_sq;
    // 7 * z = (z << 2) + (z << 1) + z
    unsigned int term2 = (z << 2) + (z << 1) + z;
    
    return (term1 + term2 + 3) & 15;
}

// Standard Quadratic LCG step function for benchmark comparison
inline unsigned int nextQCG_Standard(unsigned int z) {
    return (11 * z * z + 7 * z + 3) % 16;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Initial seed: z0 = 14
    unsigned int z_0 = 14;
    unsigned int z_current = z_0 & 15; // 14 mod 16 = 14

    cout << "=== INITIAL STATE ===\n";
    cout << "z_0 (Original): " << z_0 << "\n";
    cout << "z_0 (Mod 16):   " << z_current << " | Binary: ";
    printBinary4Bit(z_current);
    cout << "\n\n";

    cout << "=== FIRST 10 GENERATED NUMBERS (STEP-BY-STEP) ===\n";
    cout << left << setw(8) << "Step" 
         << setw(12) << "Base 10" 
         << setw(16) << "Base 2 (4-bit)" << "\n";
    cout << "-------------------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        z_current = nextQCG_Bitwise(z_current);
        cout << left << "z_" << setw(5) << i 
             << setw(12) << z_current << "| ";
        printBinary4Bit(z_current);
        cout << "\n";
    }
    cout << "\n";

    // Performance Benchmark (100,000,000 iterations)
    const int iterations = 100000000;
    volatile unsigned int dummy1 = z_0 & 15;
    volatile unsigned int dummy2 = z_0 & 15;

    auto start_bw = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy1 = nextQCG_Bitwise(dummy1);
    }
    auto end_bw = chrono::high_resolution_clock::now();
    chrono::duration<double> bw_time = end_bw - start_bw;

    auto start_std = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy2 = nextQCG_Standard(dummy2);
    }
    auto end_std = chrono::high_resolution_clock::now();
    chrono::duration<double> std_time = end_std - start_std;

    cout << "=== BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Bitwise QCG Time:  " << bw_time.count() << " seconds\n";
    cout << "Standard QCG Time: " << std_time.count() << " seconds\n";

    double speedup = std_time.count() / bw_time.count();
    cout << "\nIn C++, Bitwise operations are ~" << fixed << setprecision(2) 
         << speedup << "x FASTER!\n";

    return 0;
}
