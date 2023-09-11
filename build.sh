cd src/
g++ -c -g -Wall -std=c++20 main.cpp
g++ -c -g -Wall -std=c++20 instruction_set.cpp
cd ../
g++ -o emulator src/main.o src/instruction_set.o