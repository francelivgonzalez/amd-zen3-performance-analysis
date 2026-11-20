// CS3339 AMD Zen 3 project: illustrative MOCK demonstration only.
// Build: g++ -O2 -std=c++17 -pthread mock_demo.cpp -o mock_demo
// Usage: ./mock_demo cache 32
//        ./mock_demo memory sequential
//        ./mock_demo memory random
//        ./mock_demo multicore 4
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std;

using Clock = chrono::steady_clock;
// Only the final result is volatile, so we do not add a volatile write per element.
volatile uint64_t observable = 0;

// Returns elapsed wall-clock time in milliseconds.
template <typename F> double time_ms(F action) {
    auto start = Clock::now();
    action();
    auto end = Clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

// Pointer chasing makes each next access depend on the previous one.
// The shuffled cycle prevents a predictable sequential traversal.
void cache_demo(size_t kib) {
    if (kib < 1 || kib > 131072) { cerr << "Cache size must be 1..131072 KiB\n"; exit(1); }
    const size_t count = max<size_t>(2, kib * 1024 / sizeof(uint32_t));
    vector<uint32_t> order(count);
    iota(order.begin(), order.end(), 0);
    mt19937 rng(12345);
    shuffle(order.begin(), order.end(), rng);
    vector<uint32_t> next(count);
    for (size_t i = 0; i < count; ++i) next[order[i]] = order[(i + 1) % count];
    uint32_t pos = order[0];
    // Each size gets 2 million dependent reads. Total reads, not bytes, stay constant.
    constexpr size_t steps = 2'000'000;
    auto elapsed = time_ms([&] {
        for (size_t i = 0; i < steps; ++i) pos = next[pos];
    });
    observable = pos;
    cout << "cache," << kib << "," << elapsed << "," << elapsed * 1e6 / steps << "," << pos << "\n";
}

// Same data and same index-vector reads in both cases; only the index order changes.
// Random indices reduce spatial locality and tend to defeat prefetching.
void memory_demo(const string& pattern) {
    if (pattern != "sequential" && pattern != "random") {
        cerr << "Expected sequential or random\n"; exit(1);
    }
    constexpr size_t N = 8 * 1024 * 1024;  // 32 MiB of int data
    vector<uint32_t> data(N);
    for (size_t i = 0; i < N; ++i) data[i] = static_cast<uint32_t>(i % 251);
    vector<uint32_t> indices(N);
    iota(indices.begin(), indices.end(), 0);
    if (pattern == "random") {
        mt19937 rng(42);  // Fixed seed for repeatability.
        shuffle(indices.begin(), indices.end(), rng);
    }
    uint64_t sum = 0;
    // Repeat traversal enough times to measure rather than just timer overhead.
    constexpr int repeats = 4;
    auto elapsed = time_ms([&] {
        for (int r = 0; r < repeats; ++r) {
            for (uint32_t idx : indices) sum += data[idx];
        }
    });
    observable = sum;
    cout << "memory," << pattern << "," << elapsed << ",," << sum << "\n";
}

// Fixed total work, split across N threads. Each thread sums a disjoint range.
// At large thread counts, overhead and shared resources can reduce efficiency.
void multicore_demo(unsigned threads) {
    if (threads < 1 || threads > 128) { cerr << "Thread count must be 1..128\n"; exit(1); }
    constexpr size_t N = 16 * 1024 * 1024;  // 64 MiB total data
    vector<uint32_t> data(N);
    for (size_t i = 0; i < N; ++i) data[i] = static_cast<uint32_t>(i % 251);
    vector<uint64_t> sums(threads, 0);
    // Repeat each thread's range, keeping total number of reads fixed.
    constexpr int repeats = 12;
    auto elapsed = time_ms([&] {
        vector<thread> workers;
        workers.reserve(threads);
        for (unsigned t = 0; t < threads; ++t) {
            workers.emplace_back([&, t] {
                size_t first = N * t / threads;
                size_t last = N * (t + 1) / threads;
                uint64_t local_sum = 0;
                for (int r = 0; r < repeats; ++r) {
                    for (size_t i = first; i < last; ++i) local_sum += data[i];
                }
                sums[t] = local_sum;
            });
        }
        for (auto& worker : workers) worker.join();
    });
    uint64_t sum = accumulate(sums.begin(), sums.end(), uint64_t{0});
    observable = sum;
    cout << "multicore," << threads << "," << elapsed << ",," << sum << "\n";
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " cache <KiB> | memory <sequential|random> | multicore <threads>\n";
        return 1;
    }
    cout << fixed << setprecision(3);
    string mode = argv[1];
    if (mode == "cache") cache_demo(stoull(argv[2]));
    else if (mode == "memory") memory_demo(argv[2]);
    else if (mode == "multicore") multicore_demo(stoul(argv[2]));
    else { cerr << "Unknown mode\n"; return 1; }
    return 0;
}
