#include <iostream>
#include <random>
#include <functional>
#include <cmath>
#include <fstream>

/* 1st function constants
const double a = 0.0;
const double b = 8.0;
const double M = 2;
*/

/* 2nd function constants
const double a = 0.0;
const double b = 2 * std::acos(0.0);
const double M = 1;
*/

/* 3rd function constants */
const double a = 0.0;
const double b = 1.0;
const double M = 4;
//

/* function whose integral the program estimates */
// const std::function<double(double)> f = static_cast<double(*)(double)>(std::cbrt); // 1st function
// const std::function<double(double)> f = static_cast<double(*)(double)>(std::sin); // 2nd function
const std::function<double(double)> f = [](double x){return 4*x*(1-x)*(1-x)*(1-x);}; // 3st function

// class for generating random numbers
class Random {
	private:

	static std::random_device rd;
	static std::mt19937 mt;
	std::uniform_real_distribution<> dis;

	public:

	Random(double a, double b) : dis(a, b) {}
	double generate() {
		return dis(mt);
	}
};

std::random_device Random::rd;
std::mt19937 Random::mt{Random::rd()};

bool is_under_graph(double x_coord, double y_coord) {
	return y_coord < f(x_coord);
}

int main() {
	Random x(a, b); // x coordinate generator object
	Random y(0.0, M); // y coordinate generator object
	std::ofstream out("result.txt");
	for (int n = 50; n <= 5000; n += 50) {
		for (int k = 0; k < 50; ++k) {
			int C = 0;
			for (int i = 0; i < n; ++i) {
				double x_coord = x.generate();
				double y_coord = y.generate();
				if (is_under_graph(x_coord, y_coord)) { // check if the generated point is under the graph of the function
					++C;
				}
			}
			out << (double) C/n * (b-a) * M << " ";
		}
		out << "\n";
	}
	out.close();
}
