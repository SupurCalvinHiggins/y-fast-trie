#pragma once
#include "binary-search-tree-node.h"


template <typename T>
class BinarySearchTree {
public:
	void insert(T key);
	void remove(T key);
	bool contains(T key);
	size_t size();
};