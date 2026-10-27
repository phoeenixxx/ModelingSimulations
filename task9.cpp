/*
 * TASK PROBLEM STATEMENT:
 * -----------------------------------------------------------------------------
 * Problem 9:
 * Write a function of Linear Congruential Generators:
 *
 *     z_i = (187 * z_{i-1}) mod (2^8 - 5),   z_0 = 119
 *
 * simulation division algorithm
 * -----------------------------------------------------------------------------
 * Bitwise Simulation Breakdown:
 *   Modulo: M = 2^8 - 5 = 251
 *   Formula: X mod (2^8 - 5) => (X >> 8) * 5 + (X & 255)
 *   Multiplication: 187 * z => (z << 7) + (z << 5) + (z << 4) + (z << 3) + (z << 1) + z
 */

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;

// Helper function to print 8-bit binary numbers cleanly
inline void printBinary8Bit(unsigned int n) {
    string s = "";
    for (int i = 7; i >= 0; --i) {
        s += ((n >> i) & 1) ? "1" : "0";
    }
    cout << "0b" << s;
}

// Simulation Division Function (Unrolled Loop for Max Performance)
// Calculates: X mod 251  where  251 = 2^8 - 5
inline unsigned int bitwiseSimMod251(unsigned long long x) {
    const unsigned int MASK_8 = 255; // 2^8 - 1 = 0xFF

    // Step 1: First Pseudo-Mersenne Reduction pass
    // high * 5 + low  =>  ((high << 2) + high) + low
    unsigned long long high = x >> 8;
    unsigned long long low = x & MASK_8;
    x = ((high << 2) + high) + low;

    // Step 2: Second Reduction pass (to ensure x fits within 8 bits)
    high = x >> 8;
    low = x & MASK_8;
    x = ((high << 2) + high) + low;

    // Step 3: Final conditional subtraction
    return (x >= 251) ? static_cast<unsigned int>(x - 251) : static_cast<unsigned int>(x);
}

// LCG Step using Simulation Division and Bitwise Multiplication
inline unsigned int nextLCG_Simulation(unsigned int z) {
    // 187 * z = (z << 7) + (z << 5) + (z << 4) + (z << 3) + (z << 1) + z
    unsigned long long mult = (static_cast<unsigned long long>(z) << 7) +
        (static_cast<unsigned long long>(z) << 5) +
        (static_cast<unsigned long long>(z) << 4) +
        (static_cast<unsigned long long>(z) << 3) +
        (static_cast<unsigned long long>(z) << 1) +
        z;

    return bitwiseSimMod251(mult);
}

// Standard LCG Step for benchmark comparison
inline unsigned int nextLCG_Standard(unsigned int z) {
    return static_cast<unsigned int>((187ULL * z) % 251ULL);
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Initial seed
    unsigned int z_0 = 119;
    unsigned int z_current = bitwiseSimMod251(z_0); // 119 mod 251 = 119

    cout << "=== INITIAL STATE ===\n";
    cout << "z_0 (Original): " << z_0 << "\n";
    cout << "z_0 (Mod 251):  " << z_current << " | Binary: ";
    printBinary8Bit(z_current);
    cout << "\n\n";

    cout << "=== FIRST 10 GENERATED NUMBERS (STEP-BY-STEP) ===\n";
    cout << left << setw(8) << "Step"
        << setw(12) << "Base 10"
        << setw(16) << "Base 2 (8-bit)" << "\n";
    cout << "-------------------------------------\n";

    for (int i = 1; i <= 10; ++i) {
        z_current = nextLCG_Simulation(z_current);
        cout << left << "z_" << setw(5) << i
            << setw(12) << z_current << "| ";
        printBinary8Bit(z_current);
        cout << "\n";
    }
    cout << "\n";

    // Performance Benchmark (100,000,000 iterations)
    const int iterations = 100000000;
    volatile unsigned int dummy1 = z_current;
    volatile unsigned int dummy2 = z_current;

    auto start_sim = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy1 = nextLCG_Simulation(dummy1);
    }
    auto end_sim = chrono::high_resolution_clock::now();
    chrono::duration<double> sim_time = end_sim - start_sim;

    auto start_std = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy2 = nextLCG_Standard(dummy2);
    }
    auto end_std = chrono::high_resolution_clock::now();
    chrono::duration<double> std_time = end_std - start_std;

    cout << "=== BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Simulation Division LCG Time: " << sim_time.count() << " seconds\n";
    cout << "Standard Modulo LCG Time:     " << std_time.count() << " seconds\n";

    double speedup = std_time.count() / sim_time.count();
    cout << "\nIn C++, Simulation Division is ~" << fixed << setprecision(2)
        << speedup << "x FASTER!\n";

    return 0;
}
