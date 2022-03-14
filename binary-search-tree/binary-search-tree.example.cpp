#include "binary-search-tree.h"
#include <iostream>
#include <cstdint>

int main() {
    BinarySearchTree<int> tree;

	tree.insert(9);
    if(tree.contains(9))
        std::cout << "9 is in the tree" << std::endl;
    
    tree.remove(9);
    if (!tree.contains(9))
        std::cout << "9 is not in the tree" << std::endl;
    
    tree.remove(9);
    if (!tree.contains(9))
        std::cout << "9 can not be removed from the tree" << std::endl;
   
    tree.insert(1);
    if(tree.contains(1))
        std::cout << "1 is in the tree" << std::endl;
    tree.insert(5);
    if(tree.contains(5))
        std::cout << "5 is in the tree" << std::endl;
    tree.insert(11);
    if(tree.contains(11))
        std::cout << "11 is in the tree" << std::endl;
    tree.insert(16);
    if(tree.contains(16))
        std::cout << "16 is in the tree" << std::endl;
    
    std::cout << "Tree Size: " << tree.size() << std::endl;

}