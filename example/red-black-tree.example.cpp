#include "../src/red-black-tree/red-black-tree.h"
#include <iostream>
#include <cstdint>

int main() {
    RedBlackTree<uint32_t> tree;

    tree.insert(5);
    if (tree.contains(5))
        std::cout << "The tree contains the number 5." << std::endl;

    tree.insert(5);
    if (tree.contains(5))
        std::cout << "Double insertions are effectively no-ops." << std::endl;
    
    tree.remove(5);
    if (!tree.contains(5))
        std::cout << "The tree does not contain the number 5." << std::endl;
    
    tree.remove(5);
    if (!tree.contains(5))
        std::cout << "Double removes are effectively no-ops." << std::endl;
    
    tree.insert(7);
    tree.insert(11);
    tree.insert(16);

    std::cout << "The successor of 11 is "  << tree.successor(11).value() << "." << std::endl;
    std::cout << "The predecessor of 11 is "  << tree.predecessor(11).value() << "." << std::endl;

    std::cout << "The successor of 10 is "  << tree.successor(10).value() << "." << std::endl;
    std::cout << "The predecessor of 10 is "  << tree.predecessor(10).value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << tree.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << tree.predecessor(7).has_value() << "." << std::endl;
}