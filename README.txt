STUDENT RESULTS REGISTER - C++17

Files:
  src/StudentResults.h  DynamicArray template, file operations, sorting and search
  src/main.cpp          Demonstration application
  src/tests.cpp         Boundary and functional tests
  src/benchmark.cpp     chrono timing experiment
  data/students.txt     Ten sample student records

Compile from the project root:
  g++ -std=c++17 -Wall -Wextra -pedantic -O2 src/main.cpp -o student_register
  ./student_register

Run tests:
  g++ -std=c++17 -Wall -Wextra -pedantic -O2 src/tests.cpp -o tests
  ./tests

AddressSanitizer (where supported):
  g++ -std=c++17 -g -O1 -fsanitize=address -fno-omit-frame-pointer src/tests.cpp -o tests_asan
  ./tests_asan

Timing experiment:
  g++ -std=c++17 -O2 -DNDEBUG src/benchmark.cpp -o benchmark
  ./benchmark

IMPORTANT TASK A2:
The coursework requires the O(n log n) algorithm to depend on the LAST DIGIT
of the registration number:
  EVEN: use mergeSort(registerData, byId) in main.cpp.
  ODD:  use quickSortRandom(registerData, byId) in main.cpp.
Both implementations are included for learning and verification. Submit only
the algorithm matching your own registration-number last digit in the final
explanation and timing table.

Data file format:
  studentId|name|course|score

AI use declaration is included in the report. Personalize your own explanation,
run the tests on your machine, and verify the code before submission.
