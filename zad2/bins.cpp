// use bins_run.sh to concurrently run the experiments

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

const int multiplier = 5;

std::random_device rd;

class Random {
	private:

	const std::vector<int>::size_type buf_size = multiplier * 100'000;

	std::mt19937 mt;
	std::vector<int> buf;
	std::vector<int>::size_type index = 0;

	public:

	Random() : mt(rd()), buf(buf_size, 0) {}
	int upper;
	std::vector<int>::size_type max_generate; // the number of random numbers to generate

	void generate() { // generation in range {0, 1, ..., upper}
		//std::cout << "generating " << max_generate << " random numbers...\n";
		std::uniform_int_distribution<> dis(0, upper);
		std::generate(std::begin(buf), std::begin(buf) + max_generate, [&](){return dis(mt);});
		//std::generate(std::begin(buf) + index + 1, std::begin(buf) + max_gen, [&](){return dis(mt);});
		index = 0;
	}

	int next() {
		if (index >= max_generate) {
			//max_generate = std::max(static_cast<std::vector<int>::size_type>(1'000), max_generate / (multiplier / 100));
			generate();
		}
		//std::cout << index << " ";
		return buf[index++];
	}
};

int main(int, char **argv) {
	using vector_it = std::vector<int>::iterator;

	std::vector<int> bins(100'000, 0); // bins[i] = {how many balls inside i-th bin
	std::array<vector_it, 3> range_iterators; // iterators marking the half-sorted vector of bins with regard to the number of balls inside

	Random rng;
	int m = std::atoi(argv[1]); // begin experiment for n = m

	int balls_thrown; // how many balls thrown in so far in a single experiment
	int max_balls; // how mang balls are there maximum in a single bin

	int B_n = 0;
	int U_n = 0;
	int L_n = 0;
	int C_n = 0;
	int D_n = 0;

	for (int n = m; n < m + 10'000; n += 1'000) {

		// set generator parameters and generate numbers

		rng.max_generate = multiplier * n;
		rng.upper = n - 1;
		rng.generate();

		// set vector iterators for begin and end for current experiment

		auto bins_begin = std::begin(bins);
		auto bins_end = std::begin(bins) + n;

		for (int j = 0; j < 50; ++j) {

			// empty bins

			std::fill_n(bins_begin, n, 0);

			// fill range_iterators with end iterators, since all bins are empty

			range_iterators = {bins_end, bins_end, bins_end};
			//                 ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
			//                 {1} beg,  {2} beg,  end iter

			balls_thrown = 0;
			max_balls = 0;
			
			while (range_iterators[1] != bins_begin) {

				// choose a bin at random

				const int chosen_bin = rng.next();
			
				// sort the bins if needed and update the iterators

				const int it_index = std::min(2, bins[chosen_bin]);
				++bins[chosen_bin];
				vector_it curr_it = range_iterators[it_index];
				std::iter_swap(bins_begin + chosen_bin, curr_it - 1);

				if (it_index != 2) {
					--range_iterators[it_index];
				}

				// update the information about the number of balls

				++balls_thrown; //
				max_balls = std::max(max_balls, bins[chosen_bin]); //
				
				// gathering results

				if (it_index == 1 && range_iterators[1] == bins_end - 1) {
					B_n = balls_thrown;
				}
				if (balls_thrown == n) {
					U_n = range_iterators[0] - bins_begin;
					L_n = max_balls;
				}
				if (it_index == 0 && range_iterators[0] == bins_begin) {
					C_n = balls_thrown;
				}
			}
			D_n = balls_thrown;

			// collect results

			std::cout << B_n << " " << U_n << " " << L_n << " " << C_n << " " << D_n << " " << D_n - C_n << "\n";
		}
	}
}
