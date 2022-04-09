#include "binary-search-tree.h"
#include <iostream>
#include <cstdint>

int main() {
    BinarySearchTree<uint32_t> tree;
    BinarySearchTree<uint32_t> tree2;
    BinarySearchTree<uint32_t> merged_tree;
    
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

    std::cout << "Does the successor of 11 exist? "  << tree.successor(11).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 11 exist? "  << tree.predecessor(11).has_value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << tree.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << tree.predecessor(7).has_value() << "." << std::endl;

    tree.remove(7);
    tree.remove(11);
    tree.remove(16);

    tree.insert(10);
    tree.insert(12);
    tree.insert(15);
    tree.insert(21);
    tree.insert(11);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);
    tree.insert(4);
    tree.insert(8);
    
    std::cout << "Original BST : ";
    tree.printInOrder();
    std::cout << std::endl;

    tree.split(tree2, 10);

    std::cout << "First BST : ";
    tree.printInOrder();
    std::cout << std::endl;

    std::cout << "Second BST : ";
    tree2.printInOrder();
    std::cout << std::endl;

    std::cout << "---AFTER MERGE---" << std::endl;
    tree.merge(tree2, merged_tree);
    
    std::cout << "First BST : ";
    tree.printInOrder();
    std::cout << std::endl;

    std::cout << "Second BST : ";
    tree2.printInOrder();
    std::cout << std::endl;
    
    std::cout << "Merged BST : ";
    merged_tree.printInOrder();
    std::cout << std::endl;
    
    

}