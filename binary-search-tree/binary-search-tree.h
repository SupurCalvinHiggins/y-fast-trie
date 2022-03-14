#pragma once
#include "binary-search-tree-node.h"
#include <cstddef>

#define LEFT 0
#define RIGHT 1

//! Add Split, Merge, Pred, Succ
template <typename T>
class BinarySearchTree {
private:
	size_t size_;
	BinarySearchTreeNode<T>* root;
	BinarySearchTreeNode<T>* predecessor_node(BinarySearchTreeNode<T>* curr, T key);
	BinarySearchTreeNode<T>* successor_node(BinarySearchTreeNode<T>* curr, T key);
public:
	BinarySearchTree();
	~BinarySearchTree();
	void insert(T key);
	void insert(BinarySearchTreeNode<T>** curr, T key);
	void remove(T key);
	void remove(BinarySearchTreeNode<T>* curr, BinarySearchTreeNode<T>* parent, T key);
	bool contains(T key);
	bool contains(BinarySearchTreeNode<T>* curr, T key);
	void deleteTree(BinarySearchTreeNode<T>* curr);
	void predecessor_node(T key);
	void successor_node(T key);
	BinarySearchTreeNode<T>* findMin(BinarySearchTreeNode<T>* curr);
	BinarySearchTreeNode<T>* findMax(BinarySearchTreeNode<T>* curr);
	size_t size() {return size_; };
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree() {
	this->root = nullptr;
	this->size_ = 0;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
	deleteTree(this->root);
}

/*
template <typename T>
void BinarySearchTree<T>::predecessor_node(T key) {
	if (this->root == nullptr || !(contains(key))) return;
	predecessor_node(this->root, key);
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::predecessor_node(BinarySearchTreeNode<T>* curr, T key) {
	if (curr->key == key && curr->children[LEFT] != nullptr) {
		return findMax(curr->children[LEFT]);
	}
}

template <typename T>
void BinarySearchTree<T>::successor_node(T key) {
	if (this->root == nullptr || !(contains(key))) return;
	successor_node(this->root, key);
}

//TODO: FINISH
template <typename T>
BinarySearchTreeNode<T>* successor_node(BinarySearchTreeNode<T>* curr, T key) {

}
*/

template <typename T>
void BinarySearchTree<T>::insert(T key) {
	if (contains(key)) return;
	insert(&(this->root), key);
}

template <typename T>
void BinarySearchTree<T>::insert(BinarySearchTreeNode<T>** curr, T key) {
	if (*curr == nullptr) {
		BinarySearchTreeNode<T>* temp = new BinarySearchTreeNode<T>(key);
		*curr = temp;

		this->size_++;
	}
	else if (key < (*curr)->key) {
		insert(&(*curr)->children[LEFT], key);
	}
	else if (key > (*curr)->key) {
		insert(&(*curr)->children[RIGHT], key);
	}
}

template <typename T>
void BinarySearchTree<T>::remove(T key) {
	if (!contains(key)) return;
	remove(this->root, this->root, key);
}

template <typename T>
void BinarySearchTree<T>::remove(BinarySearchTreeNode<T>* curr, BinarySearchTreeNode<T>* parent, T key) {
	this->size_--;

	if (key < curr->key) {
		remove(curr->children[LEFT], curr, key);
	}
	else if (key > curr->key) {
		remove(curr->children[RIGHT], curr, key);
	}
	else if (curr != nullptr && curr->children[LEFT] == nullptr && curr->children[RIGHT] == nullptr) { //leaf node
		delete curr;
		curr = nullptr;
		}
	else if (curr->children[LEFT] != nullptr && curr->children[RIGHT] != nullptr) { //two children
		BinarySearchTreeNode<T>* temp = findMin(curr->children[RIGHT]);
		curr->key = temp->key;
		remove(curr->children[RIGHT], curr, curr->key);
	}
	else if (curr->children[LEFT] != nullptr) { // one child L
		parent->children[LEFT] = curr->children[LEFT];
		delete curr;
		curr = nullptr;
	}
	else if (curr->children[RIGHT] != nullptr) { // one child R
		parent->children[RIGHT] = curr->children[RIGHT];
		delete curr;
		curr = nullptr;
	}
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findMin(BinarySearchTreeNode<T>* curr) {
	if (curr == nullptr || curr->children[LEFT] == nullptr) {
		return curr;
	}
	else {
		return findMin(curr->children[LEFT]);
	}
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findMax(BinarySearchTreeNode<T>* curr) {
	if (curr == nullptr || curr->children[RIGHT] == nullptr) {
		return curr;
	}
	else {
		return findMax(curr->children[RIGHT]);
	}
}

template <typename T>
bool BinarySearchTree<T>::contains(T key) {
	return contains(this->root, key);
}

template <typename T>
bool BinarySearchTree<T>::contains(BinarySearchTreeNode<T>* curr, T key) {
	if (curr == nullptr) return false;
	else if (curr->key == key) return true;
	else if (key < curr->key) return contains(curr->children[LEFT], key);
	else if (key > curr->key) return contains(curr->children[RIGHT], key);
}

template <typename T>
void BinarySearchTree<T>::deleteTree(BinarySearchTreeNode<T>* curr) {
	if (curr != nullptr) {
		deleteTree(curr->children[LEFT]);
		deleteTree(curr->children[RIGHT]);
		delete curr;
	}
}
