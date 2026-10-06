/*
 * TASK PROBLEM STATEMENT:
 * -----------------------------------------------------------------------------
 * Problem 1:
 * Rewrite the given expression using bitwise operations:
 * 
 *     (128 * a + 3) % 2048 + b / 256
 * 
 * where:
 *     a = 1542.567
 *     b = 209129.238754
 * 
 * Calculate the result and compare performance between standard floating-point 
 * arithmetic and optimized bitwise operations.
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

// Helper function to print binary values cleanly
inline void printBinary(unsigned int n) {
    if (n == 0) {
        cout << "0b0";
        return;
    }
    string s = "";
    while (n > 0) {
        s = ((n & 1) ? "1" : "0") + s;
        n >>= 1;
    }
    cout << "0b" << s;
}

int main() {
    // Enable Fast I/O for max performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Input values from the task
    double a_dec = 1542.567;
    double b_dec = 209129.238754;

    // Integer conversion
    int a_int = static_cast<int>(a_dec); // 1542
    int b_int = static_cast<int>(b_dec); // 209129

    // Binary representation arguments
    int a_bin = 0b11000000110;          // 1542
    int b_bin = 0b110011000011011001;    // 209129

    cout << "=== INPUT REPRESENTATIONS ===\n";
    cout << "Decimal (Base 10): a = " << a_dec << ", b = " << b_dec << "\n";
    cout << "Integer (Base 10): a_int = " << a_int << ", b_int = " << b_int << "\n";
    cout << "Binary  (Base 2):  a_bin = "; printBinary(a_bin);
    cout << ", b_bin = "; printBinary(b_bin);
    cout << "\n\n";

    // 2. Step-by-Step Bitwise Breakdown
    int step1 = a_bin << 7;
    int step2 = step1 + 0b11;
    int step3 = step2 & 0b11111111111; // & 2047
    int step4 = b_bin >> 8;
    int result_bitwise = step3 + step4;

    cout << "=== STEP-BY-STEP BREAKDOWN ===\n";
    cout << "Step 1 (a << 7):   " << setw(8) << step1 << " | Binary: "; printBinary(step1); cout << "\n";
    cout << "Step 2 (+ 3):      " << setw(8) << step2 << " | Binary: "; printBinary(step2); cout << "\n";
    cout << "Step 3 (& 2047):   " << setw(8) << step3 << " | Binary: "; printBinary(step3); cout << "\n";
    cout << "Step 4 (b >> 8):   " << setw(8) << step4 << " | Binary: "; printBinary(step4); cout << "\n";
    cout << "Final Bitwise Res: " << setw(8) << result_bitwise << " | Binary: "; printBinary(result_bitwise); cout << "\n\n";

    // 3. Performance Benchmark (10,000,000 iterations for max benchmark accuracy)
    const int iterations = 10000000;
    volatile int dummy_int = 0;
    volatile double dummy_double = 0;

    // Benchmark A: Max Optimized Bitwise Operations
    auto start_bw = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy_int = (((a_bin << 7) + 3) & 2047) + (b_bin >> 8);
    }
    auto end_bw = chrono::high_resolution_clock::now();
    chrono::duration<double> bw_time = end_bw - start_bw;

    // Benchmark B: Standard Float Arithmetic
    auto start_fl = chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        dummy_double = fmod((128.0 * a_dec + 3.0), 2048.0) + (b_dec / 256.0);
    }
    auto end_fl = chrono::high_resolution_clock::now();
    chrono::duration<double> fl_time = end_fl - start_fl;

    // 4. Results
    cout << "=== C++ BENCHMARK RESULTS (" << iterations << " iterations) ===\n";
    cout << "Bitwise Execution Time: " << bw_time.count() << " seconds\n";
    cout << "Float Execution Time:   " << fl_time.count() << " seconds\n";

    double speedup = fl_time.count() / bw_time.count();
    cout << "\nIn C++, Bitwise operations are ~" << fixed << setprecision(2) 
         << speedup << "x FASTER!\n";

    return 0;
}
