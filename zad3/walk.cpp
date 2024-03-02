// zad 2. 
//
// run concurrently with run_walk.sh

#include <iostream>
#include <random>

std::random_device rd;

class Random {
	private:

	std::mt19937 mt;

	public:

	Random() : mt(rd()) {}

	int generate() {
		std::uniform_int_distribution<> dis(0, 1);
		return dis(mt);
	}
};

int main(int, char **argv) {
	int N = std::stoi(argv[1]);
	int S;
	int k = 5000;
	Random rng;
	for (int i = 0; i < k; ++i) {
		S = 0;
		for (int n = 0; n < N; ++n) {
			if (rng.generate() == 0) {
				S -= 1;
			} else {
				S += 1;
			}
		}
		std::cout << S << " ";
	}
}
