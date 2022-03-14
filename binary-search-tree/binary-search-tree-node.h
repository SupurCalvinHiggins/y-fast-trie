#pragma once
#include "binary-search-tree.h"

template <typename T>
class BinarySearchTreeNode {
private:
	T key;
	BinarySearchTreeNode<T>* children[2];
public:
	BinarySearchTreeNode(T key);
	BinarySearchTreeNode(T key, BinarySearchTreeNode<T>* children[2]);
	template <typename> friend class BinarySearchTree;
};

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode(T key){
this->children[0] = nullptr;
this->children[1] = nullptr;
this->key = key;
}

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode(T key, BinarySearchTreeNode<T>* children[2]) {
	this->key = key;
	this->children[0] = children[0];
	this->children[1] = children[1];
}