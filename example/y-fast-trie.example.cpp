#include "../src/y-fast-trie/y-fast-trie.h"
#include <iostream>
#include <cstdint>

int main() {
    YFastTrie<uint32_t> trie;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "The trie contains the number 5." << std::endl;

    trie.insert(5);
    if (trie.contains(5))
        std::cout << "Double insertions are effectively no-ops." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "The trie does not contain the number 5." << std::endl;
    
    trie.remove(5);
    if (!trie.contains(5))
        std::cout << "Double removes are effectively no-ops." << std::endl;
    
    trie.insert(7);
    trie.insert(11);
    trie.insert(16);

    std::cout << "The successor of 11 is "  << trie.successor(11).value() << "." << std::endl;
    std::cout << "The predecessor of 11 is "  << trie.predecessor(11).value() << "." << std::endl;

    std::cout << "The successor of 10 is "  << trie.successor(10).value() << "." << std::endl;
    std::cout << "The predecessor of 10 is "  << trie.predecessor(10).value() << "." << std::endl;

    std::cout << "Does the successor of 16 exist? "  << trie.successor(16).has_value() << "." << std::endl;
    std::cout << "Does the predecessor of 7 exist? "  << trie.predecessor(7).has_value() << "." << std::endl;
}