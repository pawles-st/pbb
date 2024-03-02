#include <iostream>
#include <random>
#include <functional>
#include <cmath>
#include <fstream>

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

struct Point {
	double x;
	double y;
};

double distance(const Point& p1, const Point& p2) {
	return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

int main() {
	
	// constants
	const double a = 0.0;
	const double b = 2.0;
	const double m = 0.0;
	const double M = 2.0;
	const Point center = {1.0, 1.0};
	//

	Random x(a, b); // x coordinate generator object
	Random y(m, M); // y coordinate generator object
	std::ofstream out("result.txt");
	for (int n = 50; n <= 5000; n += 50) {
		for (int k = 0; k < 50; ++k) {
			int C = 0;
			for (int i = 0; i < n; ++i) {
				const Point p = {x.generate(), y.generate()};
				if (distance(p, center) <= 1) { // check if the generated point lies within the circle with origin in (1,1) and radius 1
					++C;
				}
			}
			out << (double) C/n * (b-a) * M << " ";
		}
		out << "\n";
	}
	out.close();
}
