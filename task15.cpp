/*
 * TASK PROBLEM STATEMENT:
 * -----------------------------------------------------------------------------
 * Problem 15:
 * Consider Tausworthe generator with only two coefficients of bit type:
 * 
 *     b_i = (b_{i-r} + b_{i-q}) mod 2
 * 
 * where b_i = 0 or 1; 0 < r < q, and r = 2 and q = 5.
 * In our case b_i = 1, if i <= 5.
 * Write a function that generates the sequence {b_i}, i = 1,...,32 of bit type
 * numbers and rewrites given number in the decimal form.
 * -----------------------------------------------------------------------------
 * Bitwise Optimization Breakdown:
 *   - Addition mod 2: (x + y) % 2 => x ^ y (XOR operation)
 *   - Bit sequence assembly: decimal_value = (decimal_value << 1) | b_i
 */

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// Function to generate Tausworthe sequence {b_i} for i = 1..32 and return Decimal form
inline uint32_t generateTausworthe32(vector<int>& b_seq) {
    b_seq.resize(33); // 1-indexed for convenience (1 to 32)
    
    // Initial conditions: b_i = 1 for i <= 5
    for (int i = 1; i <= 5; ++i) {
        b_seq[i] = 1;
    }

    uint32_t decimal_value = 0;

    // Generate sequence and accumulate decimal number using bit shifts
    for (int i = 1; i <= 32; ++i) {
        if (i > 5) {
            // b_i = (b_{i-2} + b_{i-5}) mod 2  =>  b_{i-2} ^ b_{i-5}
            b_seq[i] = b_seq[i - 2] ^ b_seq[i - 5];
        }
        // Shift left and set LSB to b_i
        decimal_value = (decimal_value << 1) | static_cast<uint32_t>(b_seq[i]);
    }

    return decimal_value;
}

// Standard modulo version for benchmark comparison
inline uint32_t generateTausworthe32_Standard() {
    int b[33];
    for (int i = 1; i <= 5; ++i) b[i] = 1;

    uint32_t decimal_value = 0;
    for (int i = 1; i <= 32; ++i) {
        if (i > 5) {
            b[i] = (b[i - 2] + b[i - 5]) % 2;
        }
        decimal_value = (decimal_value * 2) + b[i];
    }
    return decimal_value;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> b_seq;
    uint32_t decimal_result = generateTausworthe32(b_seq);

    cout << "=== TAUSWORTHE GENERATOR SEQUENCE (i = 1 to 32) ===\n";
    for (int i = 1; i <= 32; ++i) {
        cout << "b_" << left << setw(2) << i << " = " << b_seq[i];
        if (i % 8 == 0) cout << "\n";
        else cout << " | ";
    }
    cout << "\n";

    cout << "=== BINARY AND DECIMAL REPRESENTATION ===\n";
    cout << "Binary  (32-bit): 0b";
    for (int i = 1; i <= 32; ++i) {
        cout << b_seq[i];
    }
    cout << "\nDecimal (Base 10): " << decimal_result << "\n";
    cout << "Hexadecimal:       0x" << hex << uppercase << decimal_result << dec << "\n\n";

    // Performance Benchmark (10,000,000 iterations)
    const int iterations = 10000000;
    vector<int> dummy_seq;
    volatile uint32_t dummy1 = 0;
    volatile uint32_t dummy2 = 0;

    auto start_bw = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy1 = generateTausworthe32(dummy_seq);
    }
    auto end_bw = chrono::high_resolution_clock::now();
    chrono::duration<double> bw_time = end_bw - start_bw;

    auto start_std = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy2 = generateTausworthe32_Standard();
    }
    auto end_std = chrono::high_resolution_clock::now();
    chrono::duration<double> std_time = end_std - start_std;

    cout << "=== BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Bitwise XOR/Shift Time: " << bw_time.count() << " seconds\n";
    cout << "Standard Modulo/Mult Time: " << std_time.count() << " seconds\n";

    double speedup = std_time.count() / bw_time.count();
    cout << "\nIn C++, Bitwise XOR & Shifts are ~" << fixed << setprecision(2) 
         << speedup << "x FASTER!\n";

    return 0;
}
