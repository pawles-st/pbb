// Paweł Leśniak
// zad. dom. nr 4
//
// this programme simulates a random walk on a given graph and computes its average cover time
// usage: run with the attached graph_run.sh file

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

//const int n = 100;
int k;
int nodes_min;
int nodes_max;

struct Node {
	int value = 0;
	std::vector<Node*> neighbours;
	bool visited = false;
	Node() {}
	Node(int v) {
		value = v;
	}
};

struct Graph {
	std::vector<Node> nodes;
	/*std::vector<bool> visited;
	Graph() {
		visited.resize(nodes_max);
		visited[0] = true;
	}*/
};

std::random_device rd;

class Random {
	private:

	std::mt19937 mt;
	std::vector<int> bin_buf;
	std::vector<int>::size_type bin_buf_size = 100'000'000;
	std::vector<int>::size_type bin_index = bin_buf_size;
	int aux_range = -1;
	std::vector<int> aux_buf;
	std::vector<int>::size_type aux_buf_size = 20'000'000;
	std::vector<int>::size_type aux_index = aux_buf_size;

	public:

	Random() : mt(rd()) {
		bin_buf.resize(bin_buf_size);
		aux_buf.resize(aux_buf_size);
	}
	int generate(int a, int b) {
		if (a == b) {
			return 0;
		}
		else if (b - a == 1) {
			if (bin_index == bin_buf_size) {
				std::uniform_int_distribution dist(a, b);
				std::generate(std::begin(bin_buf), std::end(bin_buf), [&](){return dist(mt);});
				bin_index = 0;
				//std::cout << "bin gen\n";
			}
			return bin_buf[bin_index++];
		} else {
			if (b - a == aux_range + 1) {
				std::uniform_int_distribution dist(a, b);
				return dist(mt);
			}
			if (aux_index == aux_buf_size || b - a != aux_range) {
				std::uniform_int_distribution dist(a, b);
				std::generate(std::begin(aux_buf), std::end(aux_buf), [&](){return dist(mt);});
				aux_index = 0;
				aux_range = b - a;
				//std::cout << "aux gen (" << a << " " << b << ")\n";
			}
			return aux_buf[aux_index++];
		}
	}
	void clear() {
		aux_index = aux_buf_size;
	}
};

void create_clique(Graph& graph, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				graph.nodes[i].neighbours.push_back(&graph.nodes[j]);
			}
		}
	}
}

void create_path_middle(Graph& graph, int n) {
	graph.nodes[0].neighbours.push_back(&graph.nodes[1]);
	graph.nodes[0].neighbours.push_back(&graph.nodes[2]);
	graph.nodes[1].neighbours.push_back(&graph.nodes[0]);
	graph.nodes[1].neighbours.push_back(&graph.nodes[3]);
	graph.nodes[2].neighbours.push_back(&graph.nodes[0]);
	graph.nodes[2].neighbours.push_back(&graph.nodes[4]);
	for (int i = 3; i < n - 2; ++i) {
		graph.nodes[i].neighbours.push_back(&graph.nodes[i - 2]);
		graph.nodes[i].neighbours.push_back(&graph.nodes[i + 2]);
	}
	graph.nodes[n - 2].neighbours.push_back(&graph.nodes[n - 4]);
	graph.nodes[n - 1].neighbours.push_back(&graph.nodes[n - 3]);
}

void create_path_end(Graph& graph, int n) {
	graph.nodes[0].neighbours.push_back(&graph.nodes[1]);
	for (int i = 1; i < n - 1; ++i) {
		graph.nodes[i].neighbours.push_back(&graph.nodes[i - 1]);
		graph.nodes[i].neighbours.push_back(&graph.nodes[i + 1]);
	}
	graph.nodes[n - 1].neighbours.push_back(&graph.nodes[n - 2]);
}

void create_tree(Graph& graph, int n) {
	for (int i = 0, k = 1; k < n; i *= 2, ++i) {
		const int m = k;
		for (int j = i; j < m; ++j) {
			graph.nodes[j].neighbours.push_back(&graph.nodes[k]);
			graph.nodes[k].neighbours.push_back(&graph.nodes[j]);
			++k;
			if (k == n) break;
			graph.nodes[j].neighbours.push_back(&graph.nodes[k]);
			graph.nodes[k].neighbours.push_back(&graph.nodes[j]);
			++k;
		}
	}
}

void create_lollipop(Graph& graph, int n) {

	// clique

	for (int i = 0; i < (2 * n) / 3; ++i) {
		for (int j = 0; j < (2 * n) / 3; ++j) {
			if (i != j) {
				graph.nodes[i].neighbours.push_back(&graph.nodes[j]);
			}
		}
	}

	// path

	graph.nodes[(2 * n) / 3 - 1].neighbours.push_back(&graph.nodes[(2 * n / 3)]);
	for (int i = (2 * n) / 3; i < n - 1; ++i) {
		graph.nodes[i].neighbours.push_back(&graph.nodes[i - 1]);
		graph.nodes[i].neighbours.push_back(&graph.nodes[i + 1]);
	}
	graph.nodes[n - 1].neighbours.push_back(&graph.nodes[n - 2]);
}

int main(int, char** argv) {

	// initialisation

	int gt = std::atoi(argv[1]); // set graph type
	
	k = std::atoi(argv[2]); // set repetitions

	nodes_min = std::atoi(argv[3]); // set graph nodes range
	nodes_max = std::atoi(argv[4]); //

	Random random; // rng object

	Graph graph; // create the graph nodes
	graph.nodes.resize(nodes_max);
	std::generate(std::begin(graph.nodes), std::end(graph.nodes), [x = 0]() mutable {return Node(x++);});
	graph.nodes[0].visited = true;

	void (*funcs[5])(Graph&, int); // set up the graph types creation array
	funcs[0] = *create_clique;
	funcs[1] = *create_path_middle;
	funcs[2] = *create_path_end;
	funcs[3] = *create_tree;
	funcs[4] = *create_lollipop;

	// experiments

	//for (int gt = 0; gt < 5; ++gt) {
	for (int n = nodes_min; n <= nodes_max; n += 50) {
		random.clear();

		// create the graph

		for (int i = 0; i < nodes_max; ++i) { // remove the connections
			graph.nodes[i].neighbours.clear();
		}
		funcs[gt](graph, n); // connect the nodes according to the generator function

		// start repetitions

		for (int i = 0; i < k; ++i) {

			// set nodes to nonvisited

			std::for_each(std::begin(graph.nodes) + 1, std::end(graph.nodes), [](Node& node){node.visited = false;}); // always start in 0-th node, set others as not visited

			// current node

			Node *curr = &graph.nodes[0];

			// number of moves

			unsigned long long moves = 0;

			// simulate random walk

			int visited_no = 1;
			while (visited_no != n) {
				
				// uniformly pick a neighbour

//				const std::vector<Node*> neighbours = curr->neighbours;
				//std::cout << curr->neighbours.size() - 1 << "\n";
				int idx = random.generate(0, curr->neighbours.size() - 1);
				Node *next = curr->neighbours[idx];
//				assert(curr->value != next->value);
//				std::cout << "[" << curr->value << " " << next->value << "]\n";

				// check if node has been visited

				if (next->visited == false) {
					next->visited = true;
					++visited_no;
				}

				// set current node as the one moved to

				curr = next;

				// increment the number of moves

				++moves;
			}

			std::cout << moves << " ";
		}
		std::cout << "\n";
	}
	//}
}
