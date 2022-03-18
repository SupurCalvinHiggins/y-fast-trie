#pragma once
#include "binary-search-tree-node.h"
#include <cstddef>
#include <queue>
#include <iostream>

#define LEFT 0
#define RIGHT 1

//TODO: Add Split, Merge
//TODO: MAKKE COPY AND TRY TO CONVERT FUNCTIONS TO ITERATIVE
//TODO: Add Order based vectorizer
	//* Add a vectorizer function that represents tree as a vector... level order* preorder/postorder/inorder (how to sort like one in class)
template <typename T>
class BinarySearchTree {
private:
	size_t size_;
	BinarySearchTreeNode<T>* root;
	BinarySearchTreeNode<T>** findNode(BinarySearchTreeNode<T>** curr, T Key);
	BinarySearchTreeNode<T>* findMin(BinarySearchTreeNode<T>* curr);
	BinarySearchTreeNode<T>* findMax(BinarySearchTreeNode<T>* curr);
public:
	BinarySearchTree();
	//~BinarySearchTree();
	void insert(T key);
	void remove(T key);
	bool contains(T key);
	void split(BinarySearchTreeNode<T>** root2);
	void merge(BinarySearchTreeNode<T>** root2);
	BinarySearchTreeNode<T>* predecessor(T key);
	BinarySearchTreeNode<T>* successor(T key);
	BinarySearchTreeNode<T>* getRoot() { return this->root; };
	size_t size() { return size_; };
	const T limit() { return -1; }
	void printInOrder(BinarySearchTreeNode<T>* curr);
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree() {
	this->root = nullptr;
	this->size_ = 0;
}
/*
template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
	BinarySearchTreeNode<T>** curr = &(this->root);
	if (*curr == nullptr) return;
	
	std::queue<BinarySearchTreeNode<T>**> destruct;
	destruct.push(curr);

	while(!destruct.empty()) {
		curr = destruct.front();
		destruct.pop();

		if ((*curr)->children[LEFT] != nullptr) destruct.push(&(*curr)->children[LEFT]);
		if ((*curr)->children[RIGHT] != nullptr) destruct.push(&(*curr)->children[RIGHT]);

		delete *curr;
		*curr = nullptr;
		this->size_--;
	}
}
*/

template <typename T>
void BinarySearchTree<T>::insert(T key) {
	if (contains(key)) return;
	
	BinarySearchTreeNode<T>** curr = &(this->root);

	while ( *curr != nullptr ) {
		if (key < (*curr)->key )
			curr = &(*curr)->children[LEFT];
		else if (key > (*curr)->key)
			curr = &(*curr)->children[RIGHT];
	}

	BinarySearchTreeNode<T>* temp = new BinarySearchTreeNode<T>(key);
	*curr = temp;
	this->size_++;
}

template <typename T>
void BinarySearchTree<T>::remove(T key) {
	if (!contains(key)) return;

	BinarySearchTreeNode<T>** curr = &(this->root);
	BinarySearchTree<T>* parent = nullptr;
	BinarySearchTreeNode<T>** temp;

	while (*curr != nullptr && (*curr)->key != key) {
		parent = *curr;
		if ((*curr)->key > key)
			curr = &(*curr)->children[LEFT];
		else if ((*curr)->key < key)
			curr = &(*curr)->children[RIGHT];		
	}
	if (*curr == nullptr) return;

	if ((*curr)->children[LEFT] == nullptr || (*curr)->children[RIGHT] == nullptr ) { // leaf node or one child
		if ((*curr)->children[LEFT] != nullptr) // one child L
			temp = &(*curr)->children[LEFT];
		else if ((*curr)->children[RIGHT] != nullptr) // one child R
			temp = &(*curr)->children[RIGHT];
	}
}

/* Iterative remove not Working
template <typename T>
void BinarySearchTree<T>::remove(T key) {
	if (!contains(key)) return;

	BinarySearchTreeNode<T>** curr = &(this->root);
	BinarySearchTreeNode<T>** parent = &(this->root);

	curr = findNode(curr, key);
	parent = findParent(curr, key);

	if ((*curr)->children[LEFT] != nullptr && (*curr)->children[RIGHT] != nullptr) { //two children
		BinarySearchTreeNode<T>* temp = findMin((*curr)->children[RIGHT]);
		(*curr)->key = temp->key;

		curr = findNode(&(*curr)->children[RIGHT], (*curr)->key);
		parent = findParent(&(*curr)->children[RIGHT], (*curr)->key);
	}
	if (*curr != nullptr && (*curr)->children[LEFT] == nullptr && (*curr)->children[RIGHT] == nullptr) { //leaf node
		delete *curr;
		*curr = nullptr;
		}
	else if ((*curr)->children[LEFT] != nullptr) { // one child L
		(*parent)->children[LEFT] = (*curr)->children[LEFT];
		delete *curr;
		*curr = nullptr;
	}
	else if ((*curr)->children[RIGHT] != nullptr) { // one child R
		(*parent)->children[RIGHT] = (*curr)->children[RIGHT];
		delete *curr;
		*curr = nullptr;
	}
	this->size_--;
}
*/

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findMin(BinarySearchTreeNode<T>* curr) {
	if (this->size_ == 0) return nullptr;
	
	while (curr->children[LEFT] != nullptr) {
		curr = curr->children[LEFT];
	}
	
	return curr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findMax(BinarySearchTreeNode<T>* curr) {
	if (this->size_ == 0) return nullptr;

	while (curr->children[RIGHT] != nullptr) {
		curr = curr->children[RIGHT];
	}
	
	return curr;
}

//! COuld be an issue since getting freeing pointer error
template <typename T>
BinarySearchTreeNode<T>** BinarySearchTree<T>::findNode(BinarySearchTreeNode<T>** curr, T key) {
	while (*curr != nullptr && (*curr)->key != key) {
		if ((*curr)->key > key)
			curr = &(*curr)->children[LEFT];
		else if ((*curr)->key < key)
			curr = &(*curr)->children[RIGHT];		
	}

	if (*curr == nullptr) return nullptr;
	if ((*curr)->key == key) return curr;
}

template <typename T>
bool BinarySearchTree<T>::contains(T key) {
	if (findNode(&(this->root), key) == nullptr) return false;

	return true;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::predecessor(T key) {
	if (!contains(key)) return nullptr;
	
	BinarySearchTreeNode<T>* curr = this->root;
	BinarySearchTreeNode<T>* target = *findNode(&curr, key);

	if (target->children[LEFT] != nullptr) return findMax(target->children[LEFT]);
	else {
		BinarySearchTreeNode<T>* pred = nullptr;

		while (curr != target) {
			if (curr->key < target->key) {
				pred = curr;
				curr = curr->children[RIGHT];
			}
			else if (curr->key > target->key)
				curr = curr->children[LEFT];
		}
		return pred;
	}
}


template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::successor(T key) {
	if (!contains(key)) return nullptr;

	BinarySearchTreeNode<T>* curr = this->root;
	BinarySearchTreeNode<T>* target = *findNode(&curr, key);

	if (target->children[RIGHT] != nullptr)
		return findMin(target->children[RIGHT]);
	else {
		BinarySearchTreeNode<T>* succ = nullptr;

		while (curr != target) {
			if (curr->key > target->key) {
				succ = curr;
				curr = curr->children[LEFT];
			}
			else if (curr->key < target->key)
				curr = curr->children[RIGHT];
		}
		return succ;
	}
}

/*
//TODO Fix split
template <typename T>
void BinarySearchTree<T>::split(BinarySearchTreeNode<T>** root2) {
	BinarySearchTreeNode<T>** curr = &(this->root);
	if (*curr == nullptr) return;

	std::queue<BinarySearchTreeNode<T>**> tree_splitter;
	if ((*curr)->children[LEFT] != nullptr ) tree_splitter.push(&((*curr)->children[LEFT]));

	while(!tree_splitter.empty()) {
		curr = tree_splitter.front();
		tree_splitter.pop();

		if ((*curr)->children[LEFT] != nullptr ) tree_splitter.push(&(*curr)->children[LEFT]);
		if ((*curr)->children[RIGHT] != nullptr ) tree_splitter.push(&(*curr)->children[RIGHT]);

		std::cout << "INS" << std::endl;
		insert(root2, (*curr)->key);
	}
}


//TODO Reverse split when completed working implementation
template <typename T>
void BinarySearchTree<T>::merge(BinarySearchTreeNode<T>** root, BinarySearchTree<T>** root2) {
}
*/

template <typename T>
void BinarySearchTree<T>::printInOrder(BinarySearchTreeNode<T>* curr) {
	if (curr == nullptr) return;

	printInOrder(curr->children[LEFT]);
	std::cout << curr->key << " ";
	printInOrder(curr->children[RIGHT]);
}