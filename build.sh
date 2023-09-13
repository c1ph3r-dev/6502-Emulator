cd src/
g++ -c -g -Wall -std=c++20 main.cpp
g++ -c -g -Wall -std=c++20 instruction_set.cpp
g++ -c -g -std=c++20 tests.cpp
cd ../
g++ -o emulator src/main.o src/instruction_set.o src/tests.o