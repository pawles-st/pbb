// zad. 3.
//
// run concurrently with run_walk2.sh

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
	int N = std::stod(argv[1]);
	int k = 10000;
	int S;
	int L;
	bool D;
	Random rng;
	for (int i = 0; i < k; ++i) {
		S = 0;
		L = 0;
		for (int n = 0; n < N; ++n) {
			D = false;
			if (S > 0) {
				++L;
				D = true;
			}
			if (rng.generate() == 0) {
				S -= 1;
			} else {
				S += 1;
			}
			if (!D && S > 0) {
				L += 1;
			}
		}
		std::cout << static_cast<float>(L) / N << " ";
	}
}
