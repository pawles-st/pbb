// zad. 1.
//
// generates 0-1 sequence with mt

#include <fstream>
#include <iostream>
#include <random>

int main() {
	std::ofstream out("mt.txt");
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(0, 1);
	for (int i = 0; i < 1'100'000; ++i) {
		out << dis(mt);
	}
}
