#include "binary-search-tree.h"
#include <iostream>
#include <cstdint>

int main() {
    BinarySearchTree<uint32_t> tree;
    
    tree.insert(10);
    tree.insert(5);
    tree.insert(7);
    tree.insert(8);
    tree.insert(11);
    tree.insert(15);
    tree.insert(18);

    std::cout << "Tree Size: " << tree.size() << std::endl;
    
    std::cout << "ROOT: " << tree.getRoot()->getKey();

    std::cout << "\nTree: ";
    tree.printInOrder(tree.getRoot());
}