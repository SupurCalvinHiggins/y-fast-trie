#include "red-black-tree.h"

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
	RedBlackTree<int> tree = RedBlackTree<int>();
	unsigned int indices = keys.size();
	for (unsigned int i = 0; i < indices; i++) {
		std::cout << "\nInserting " << std::to_string(keys[i]) << "\n\n";
		tree.insert(keys[i]);
		tree.show(tree.root(), 0);
		std::cout << '\n';
		if (!tree.check_balance()) {
			std::cout << "Tree is unbalanced.Stopping tests" << '\n';
			break;
		}
	}

	while (indices > 0 && tree.check_balance()) {
		int index = rand() % (indices);
		int key = keys[index];
		if (key == 240) {
			key = 240;
		}
		keys.erase(keys.begin() + index);
		std::cout << "\nRemoving " << std::to_string(key) << "\n\n";
		tree.remove(key);
		if (tree.root() != nullptr) {
			tree.show(tree.root(), 0);
		}
		indices--;

		
	}
	std::cout << '\n';
	std::cout << std::to_string(tree.root()->key()) << '\n';
}