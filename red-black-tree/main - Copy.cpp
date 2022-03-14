#include "RBTree.h"

const int SEED = 0;
const int TESTS = 10000;
const int MAX = 1000;


std::vector<int> get_keys() {
	int i = 0;
	std::vector<int> keys = std::vector<int>();
	while (i <= TESTS) {
		keys.push_back(rand() % MAX);
		i++;
	}
	return keys;
}

int main(void) {
	srand(SEED);
	std::vector<int> keys = get_keys();
	RBTree<int> tree = RBTree<int>();
	for (int key : keys) {
		std::cout << "Inserting " << std::to_string(key) << "\n\n";
		tree.insert(key);
		//tree.show(tree.root(), 0);
		std::cout << '\n';
	}

	int indices = keys.size();

	while (indices > 0) {
		int key = keys[(rand() % (indices))];
		std::cout << "Removing " << std::to_string(key) << "\n\n";
		tree.remove(key);
		//tree.show(tree.root(), 0);
		
		indices--;
	}

	std::cout << '\n';
	std::cout << std::to_string(tree.root()->key()) << '\n';
}