#include "StudentResults.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

using Clock = std::chrono::steady_clock;

DynamicArray<StudentResult> makeData(int n, unsigned seed) {
    DynamicArray<StudentResult> a;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, n * 20);
    for (int i = 0; i < n; ++i) {
        int id = dist(rng);
        a.pushBack({id, "Student" + std::to_string(i), "BCS",
                    static_cast<double>(id % 100)});
    }
    return a;
}

template <typename SortFunction>
double averageMilliseconds(int n, SortFunction sortFunction) {
    double total = 0.0;
    for (int repeat = 0; repeat < 3; ++repeat) {
        auto data = makeData(n, static_cast<unsigned>(2026 + repeat));
        auto start = Clock::now();
        sortFunction(data);
        auto stop = Clock::now();
        total += std::chrono::duration<double, std::milli>(stop - start).count();
    }
    return total / 3.0;
}

int main() {
    auto byId = [](const StudentResult& a, const StudentResult& b) {
        return a.studentId < b.studentId;
    };
    const int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000};

    std::cout << "n,insertion_ms,merge_ms,random_quick_ms\n";
    for (int n : sizes) {
        const double insertion = averageMilliseconds(n,
            [&](DynamicArray<StudentResult>& a) { insertionSort(a, byId); });
        const double merge = averageMilliseconds(n,
            [&](DynamicArray<StudentResult>& a) { mergeSort(a, byId); });
        const double quick = averageMilliseconds(n,
            [&](DynamicArray<StudentResult>& a) { quickSortRandom(a, byId); });

        std::cout << n << ',' << std::fixed << std::setprecision(3)
                  << insertion << ',' << merge << ',' << quick << '\n';
    }
}
