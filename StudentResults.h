#ifndef STUDENT_RESULTS_H
#define STUDENT_RESULTS_H

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>

// One record in the Student Results Register.
struct StudentResult {
    int studentId{};
    std::string name;
    std::string course;
    double score{};
};

// A template dynamic array. It owns a raw array and doubles capacity when full.
// No STL container is used to implement the primary data structure.
template <typename T>
class DynamicArray {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow() {
        const std::size_t newCapacity = (capacity_ == 0) ? 2 : capacity_ * 2;
        T* newData = new T[newCapacity];

        for (std::size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }

        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    // Start empty; allocate storage only when the first record is inserted.
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

    // Deep-copy constructor: the new object owns its own independent array.
    DynamicArray(const DynamicArray& other)
        : data_(other.capacity_ ? new T[other.capacity_] : nullptr),
          size_(other.size_), capacity_(other.capacity_) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // Copy assignment also performs a deep copy (copy-and-swap style).
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            T* newData = other.capacity_ ? new T[other.capacity_] : nullptr;
            for (std::size_t i = 0; i < other.size_; ++i) {
                newData[i] = other.data_[i];
            }
            delete[] data_;
            data_ = newData;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    // Destructor releases the dynamically allocated array.
    ~DynamicArray() {
        delete[] data_;
    }

    // Add at the end. Capacity doubles only when the array is full.
    void pushBack(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        data_[size_++] = value;
    }

    // Remove by index. Invalid positions are rejected safely.
    bool removeAt(std::size_t index) {
        if (index >= size_) {
            return false;
        }
        for (std::size_t i = index; i + 1 < size_; ++i) {
            data_[i] = data_[i + 1];
        }
        --size_;
        return true;
    }

    // Checked access prevents reading outside the valid records.
    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Student record index is out of range.");
        }
        return data_[index];
    }

    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Student record index is out of range.");
        }
        return data_[index];
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
};

// Save records in a simple pipe-separated text file.
// Names and courses in the sample data do not contain the pipe character.
inline bool saveRecords(const DynamicArray<StudentResult>& records,
                        const std::string& filename) {
    std::ofstream out(filename);
    if (!out) return false;

    for (std::size_t i = 0; i < records.size(); ++i) {
        const StudentResult& r = records.at(i);
        out << r.studentId << '|'
            << r.name << '|'
            << r.course << '|'
            << r.score << '\n';
    }
    return static_cast<bool>(out);
}

// Load a file. Existing records are cleared only after the file opens.
// Malformed lines are skipped; valid lines are appended.
inline bool loadRecords(DynamicArray<StudentResult>& records,
                        const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    DynamicArray<StudentResult> loaded;
    std::string line;
    while (std::getline(in, line)) {
        const std::size_t p1 = line.find('|');
        const std::size_t p2 = (p1 == std::string::npos) ? p1 : line.find('|', p1 + 1);
        const std::size_t p3 = (p2 == std::string::npos) ? p2 : line.find('|', p2 + 1);
        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos)
            continue;

        try {
            StudentResult r;
            r.studentId = std::stoi(line.substr(0, p1));
            r.name = line.substr(p1 + 1, p2 - p1 - 1);
            r.course = line.substr(p2 + 1, p3 - p2 - 1);
            r.score = std::stod(line.substr(p3 + 1));
            loaded.pushBack(r);
        } catch (const std::exception&) {
            // Skip a malformed numeric field rather than crashing.
        }
    }
    records = loaded;
    return true;
}

// Stable insertion sort. The comparator defines the required ordering.
template <typename T, typename Compare>
void insertionSort(DynamicArray<T>& a, Compare comp) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        T key = a.at(i);
        std::size_t j = i;

        // Shift only when key is strictly before the previous element.
        // Equal-key elements therefore retain their original relative order.
        while (j > 0 && comp(key, a.at(j - 1))) {
            a.at(j) = a.at(j - 1);
            --j;
        }
        a.at(j) = key;
    }
}

// Merge sort helper. It uses a temporary raw array, not an STL container.
template <typename T, typename Compare>
void mergeRange(DynamicArray<T>& a, T* temp, std::size_t left,
                std::size_t mid, std::size_t right, Compare comp) {
    std::size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (!comp(a.at(j), a.at(i))) temp[k++] = a.at(i++);
        else temp[k++] = a.at(j++);
    }
    while (i < mid) temp[k++] = a.at(i++);
    while (j < right) temp[k++] = a.at(j++);
    for (std::size_t p = left; p < right; ++p) a.at(p) = temp[p];
}

template <typename T, typename Compare>
void mergeSortRange(DynamicArray<T>& a, T* temp, std::size_t left,
                    std::size_t right, Compare comp) {
    if (right - left <= 1) return;
    const std::size_t mid = left + (right - left) / 2;
    mergeSortRange(a, temp, left, mid, comp);
    mergeSortRange(a, temp, mid, right, comp);
    mergeRange(a, temp, left, mid, right, comp);
}

template <typename T, typename Compare>
void mergeSort(DynamicArray<T>& a, Compare comp) {
    if (a.size() < 2) return;
    T* temp = new T[a.size()];
    mergeSortRange(a, temp, 0, a.size(), comp);
    delete[] temp;
}

// Random-pivot quicksort helper, included for odd registration-number digits.
template <typename T, typename Compare>
std::size_t partitionRandom(DynamicArray<T>& a, std::size_t low,
                            std::size_t high, Compare comp,
                            std::mt19937& rng) {
    std::uniform_int_distribution<std::size_t> dist(low, high);
    const std::size_t pivotIndex = dist(rng);
    std::swap(a.at(pivotIndex), a.at(high));
    T pivot = a.at(high);

    std::size_t boundary = low;
    for (std::size_t i = low; i < high; ++i) {
        if (!comp(pivot, a.at(i))) {
            std::swap(a.at(boundary), a.at(i));
            ++boundary;
        }
    }
    std::swap(a.at(boundary), a.at(high));
    return boundary;
}

template <typename T, typename Compare>
void quickSortRange(DynamicArray<T>& a, std::size_t low, std::size_t high,
                    Compare comp, std::mt19937& rng) {
    if (low >= high) return;
    const std::size_t p = partitionRandom(a, low, high, comp, rng);
    if (p > low) quickSortRange(a, low, p - 1, comp, rng);
    if (p < high) quickSortRange(a, p + 1, high, comp, rng);
}

template <typename T, typename Compare>
void quickSortRandom(DynamicArray<T>& a, Compare comp) {
    if (a.size() < 2) return;
    std::random_device rd;
    std::mt19937 rng(rd());
    quickSortRange(a, 0, a.size() - 1, comp, rng);
}

// Iterative binary search. The array must already be sorted by the same key.
template <typename T, typename Key, typename KeyOf>
int binarySearch(const DynamicArray<T>& a, const Key& key, KeyOf keyOf) {
    std::size_t low = 0;
    std::size_t high = a.size(); // exclusive upper bound

    while (low < high) {
        const std::size_t mid = low + (high - low) / 2;
        if (keyOf(a.at(mid)) == key) return static_cast<int>(mid);
        if (keyOf(a.at(mid)) < key) low = mid + 1;
        else high = mid;
    }
    return -1;
}

#endif
