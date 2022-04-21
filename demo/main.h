#pragma once
#include "src/y-fast-trie/y-fast-trie.h"
#include <fstream>
#include <random>
#include <iostream>

void UPDATE_GUI();

static YFastTrie<uint8_t> yfast;

void UPDATE_GUI() {
    std::cout << "*** DOT FILE ***\n";
    std::cout << yfast.to_dot() << "\n\n";
}