#include "red-black-tree.h"
#include <cstdint>

const int SEED = 69;
const int TESTS = 100;
const int MAX = 100000;

const bool TRUE_RAND = true;
using T = std::uint8_t;


std::vector<T> get_keys() {
	int i = 0;
	std::vector<T> keys = std::vector<T>();
	while (i < TESTS) {
		keys.push_back(rand() % MAX);
		i++;
	}
	return keys;
}

int main(void) {
	if (TRUE_RAND){
		srand(std::time(nullptr));
	}
	else{
		srand(SEED);
	}
	
	std::vector<T> keys = get_keys();
	RedBlackTree<uint8_t> tree = RedBlackTree<uint8_t>();
	unsigned int indices = keys.size();
	for (unsigned int i = 0; i < indices; i++) {
		std::cout << "Size: " << std::to_string(tree.size()) << '\n';
		std::cout << "\nInserting " << std::to_string(keys[i]) << "\n\n";
		tree.insert(keys[i]);
		//tree.show(tree.root(), 0);
		std::cout << '\n';
	}

	while (indices > 0) {
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
	tree = RedBlackTree<uint8_t>();
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
	}


	T max = *tree.
	max();
	std::cout << '\n';
	bool no_double_red = tree.no_consecutive_reds();
	std::vector<std::vector<Node<T>*>> layers = tree.make_2d_vector();
	std::vector<T> layer_heights;
	layer_heights.reserve(layers.size());
	
	for (std::vector<Node<T>*>layer : layers) {
		layer_heights.push_back(layer.size());
	}

	tree.show_layers();
	std::cout << '\n';
	tree.show();
	std::cout << '\n';

	std::vector<T> colors;
	for (Node<T>* node : tree.nodes()){
		colors.push_back(node->color());
	}

	std::pair<RedBlackTree<uint8_t>*,RedBlackTree<uint8_t>*> trees = tree.split(keys[0]);
	T max1 = *trees.first->max();
	T max2 = *trees.second->max();
	std::cout << '\n';
	std::cout << std::to_string(trees.first->no_consecutive_reds()) << '\n';
	std::vector<std::vector<Node<T>*>> layers1 = trees.first->make_2d_vector();
	trees.first->show_layers();
	std::cout << '\n';
	trees.second->show();
	std::cout << '\n';
	trees.first->merge(trees.second);
	std::cout << trees.first->root() << '\n';
	std::cout << '\n';
	trees.first->show();

	RedBlackTree<T> tree3 = RedBlackTree<T>();
	for (T i = 0; i < 40; i +=2){
		if (i == 39){
			i = 39;
		}
		tree3.insert(i);
		tree3.show();
		std::cout << '\n';
	}
	for (T i = 0; i < 40; i++){
		if (i == 2){
			i = 2;
		}
		std::cout << "Successor of " + std::to_string(i) + " is " + std::to_string(*tree3.successor(i)) << '\n';
		std::cout << "Predecessor of " + std::to_string(i) + " is " + std::to_string(*tree3.predecessor(i)) << '\n';
	}
	std::cout << '\n';
	tree3.show();

	return 0;
}