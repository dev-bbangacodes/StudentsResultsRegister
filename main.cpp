#include "StudentResults.h"
#include <iomanip>
#include <iostream>

int main() {
    DynamicArray<StudentResult> registerData;

    // Load the sample register. The path is relative to the project root.
    if (!loadRecords(registerData, "data/students.txt")) {
        std::cerr << "Could not open data/students.txt\n";
        return 1;
    }

    std::cout << "STUDENT RESULTS REGISTER\n";
    std::cout << "Loaded records: " << registerData.size()
              << " | Capacity: " << registerData.capacity() << "\n\n";

    // Display records before sorting.
    for (std::size_t i = 0; i < registerData.size(); ++i) {
        const auto& s = registerData.at(i);
        std::cout << s.studentId << " | " << s.name << " | "
                  << s.course << " | " << std::fixed << std::setprecision(1)
                  << s.score << '\n';
    }

    // Task A2: sort by student ID. Use the algorithm required by your
    // registration-number last digit. Both implementations are available.
    auto byId = [](const StudentResult& a, const StudentResult& b) {
        return a.studentId < b.studentId;
    };

    // DEFAULT: Merge sort. If your registration number ends in an ODD digit,
    // replace the next line with quickSortRandom(registerData, byId).
    mergeSort(registerData, byId);

    std::cout << "\nSORTED BY STUDENT ID\n";
    for (std::size_t i = 0; i < registerData.size(); ++i) {
        const auto& s = registerData.at(i);
        std::cout << s.studentId << " | " << s.name << " | " << s.score << '\n';
    }

    // Binary search by the same key used for sorting.
    const int searchId = 1005;
    const int found = binarySearch(registerData, searchId,
        [](const StudentResult& s) { return s.studentId; });

    if (found >= 0) {
        std::cout << "\nSearch " << searchId << ": FOUND - "
                  << registerData.at(static_cast<std::size_t>(found)).name << '\n';
    } else {
        std::cout << "\nSearch " << searchId << ": NOT FOUND\n";
    }

    // Save the updated/sorted register.
    if (!saveRecords(registerData, "data/students.txt")) {
        std::cerr << "Could not save data/students.txt\n";
        return 1;
    }
    std::cout << "Updated records saved successfully.\n";
    return 0;
}
