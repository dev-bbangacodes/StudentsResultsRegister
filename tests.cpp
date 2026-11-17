#include "StudentResults.h"
#include <iostream>
#include <string>

int main() {
    int passed = 0, failed = 0;
    auto check = [&](const std::string& id, bool condition, const std::string& detail) {
        std::cout << id << " | " << (condition ? "PASS" : "FAIL")
                  << " | " << detail << '\n';
        if (condition) ++passed; else ++failed;
    };

    auto byId = [](const StudentResult& a, const StudentResult& b) {
        return a.studentId < b.studentId;
    };

    DynamicArray<StudentResult> a;
    check("T01", a.empty() && a.size() == 0, "Empty structure starts with size 0");

    a.pushBack({101, "Amina", "BCS", 78});
    check("T02", a.size() == 1 && a.at(0).studentId == 101,
          "Insert and retrieve one record");

    a.pushBack({103, "Brian", "BIT", 65});
    a.pushBack({102, "Carol", "BCS", 82});
    check("T03", a.size() == 3 && a.capacity() >= 3,
          "Capacity grows when the array becomes full");

    a.pushBack({102, "David", "BIT", 74});
    insertionSort(a, byId);
    check("T04", a.at(0).studentId == 101 && a.at(1).studentId == 102
                 && a.at(2).studentId == 102 && a.at(3).studentId == 103,
          "Insertion sort handles duplicate keys and orders ascending");

    // Stability: Carol was inserted before David and remains before David.
    check("T05", a.at(1).name == "Carol" && a.at(2).name == "David",
          "Insertion sort preserves order of equal keys");

    check("T06", binarySearch(a, 102,
          [](const StudentResult& s) { return s.studentId; }) >= 0,
          "Binary search finds an existing key");

    check("T07", binarySearch(a, 999,
          [](const StudentResult& s) { return s.studentId; }) == -1,
          "Binary search returns -1 for absent key");

    check("T08", a.removeAt(0) && a.at(0).studentId == 102,
          "Remove first record");

    check("T09", a.removeAt(a.size() - 1) && a.size() == 2,
          "Remove last record");

    check("T10", !a.removeAt(99), "Reject out-of-range removal safely");

    DynamicArray<StudentResult> empty;
    check("T11", !empty.removeAt(0), "Reject removal from empty structure");

    DynamicArray<StudentResult> copy = a;
    copy.at(0).name = "Changed copy";
    check("T12", a.at(0).name != copy.at(0).name,
          "Deep copy keeps original independent");

    DynamicArray<StudentResult> reversed;
    reversed.pushBack({5, "E", "BCS", 50});
    reversed.pushBack({4, "D", "BCS", 60});
    reversed.pushBack({3, "C", "BCS", 70});
    reversed.pushBack({2, "B", "BCS", 80});
    reversed.pushBack({1, "A", "BCS", 90});
    mergeSort(reversed, byId);
    check("T13", reversed.at(0).studentId == 1 && reversed.at(4).studentId == 5,
          "Merge sort orders reversed input");

    DynamicArray<StudentResult> already;
    already.pushBack({1, "A", "BCS", 50});
    already.pushBack({2, "B", "BCS", 60});
    already.pushBack({3, "C", "BCS", 70});
    insertionSort(already, byId);
    check("T14", already.at(0).studentId == 1 && already.at(2).studentId == 3,
          "Insertion sort handles already-sorted input");

    const std::string file = "test_students.txt";
    check("T15", saveRecords(a, file), "Save records to text file");
    DynamicArray<StudentResult> loaded;
    check("T16", loadRecords(loaded, file) && loaded.size() == a.size(),
          "Load records from text file");

    std::cout << "\nSUMMARY: " << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
