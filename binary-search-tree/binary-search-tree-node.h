#pragma once
#include "binary-search-tree.h"

// TODO: friend class
template <typename T>
class BinarySearchTreeNode {
public:
	T key;
	BinarySearchTreeNode<T>* children[2];
};