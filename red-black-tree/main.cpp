#include "red-black-tree.h"

const int SEED = 69;
const int TESTS = 100;
const int MAX = 100000;


std::vector<int> get_keys() {
	int i = 0;
	std::vector<int> keys = std::vector<int>();
	while (i < TESTS) {
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
		std::cout << "Size: " << std::to_string(tree.size()) << '\n';
		std::cout << "\nInserting " << std::to_string(keys[i]) << "\n\n";
		tree.insert(keys[i]);
		//tree.show(tree.root(), 0);
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
		std::cout << "Size: " << std::to_string(tree.size()) << '\n';
		std::cout << "\nRemoving " << std::to_string(key) << "\n\n";
		tree.remove(key);
		/*if (tree.root() != nullptr) {
			tree.show(tree.root(), 0);
		}*/
		indices--;

		
	}
	std::cout << tree.size() << std::endl; 
	std::cout << '\n';

	keys = get_keys();
	tree = RedBlackTree<int>();
	indices = keys.size();
	for (unsigned int i = 0; i < indices; i++) {
		std::cout << "Size: " << std::to_string(tree.size()) << '\n';
		std::cout << "\nInserting " << std::to_string(keys[i]) << "\n\n";
		tree.insert(keys[i]);
		//tree.show(tree.root(), 0);
		std::cout << '\n';
		if (i == 99 ){
			bool t = true;
		}
		if (!tree.check_balance()) {
			std::cout << "Tree is unbalanced.Stopping tests" << '\n';
			break;
		}
	}

	std::vector<int> colors;
	for (Node<int>* node : tree.nodes()){
		colors.push_back(node->color());
	}

	std::pair<RedBlackTree<int>,RedBlackTree<int>> trees = tree.split(keys[0]);
	int max1 = trees.first.max();
	std::cout << trees.second.max();
	trees.first.show();
	trees.second.show();
}