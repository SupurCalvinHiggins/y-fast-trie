#pragma once
#include "binary-search-tree-node.h"
#include <cstddef>
#include <optional>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>

#define LEFT 0
#define RIGHT 1

template <typename T>
class BinarySearchTree {
private:
	size_t size_;
	BinarySearchTreeNode<T>* root;
	BinarySearchTreeNode<T>* findNode(BinarySearchTreeNode<T>* curr, T Key);
	BinarySearchTreeNode<T>* findParent(BinarySearchTreeNode<T>* curr, T key);
	BinarySearchTreeNode<T>* min(BinarySearchTreeNode<T>* curr);
	BinarySearchTreeNode<T>* max(BinarySearchTreeNode<T>* curr);
	BinarySearchTreeNode<T>* predecessor_node(T key);
	BinarySearchTreeNode<T>* successor_node(T key);
	BinarySearchTreeNode<T>* generateBST(std::vector<T> &bst_data, int start, int end);
	void inOrderVector(std::vector<T> &bst_data);
public:
	BinarySearchTree();
	~BinarySearchTree();
	void insert(T key);
	void remove(T key);
	bool contains(T key);
	void split(BinarySearchTree<T> &tree2, T key);
	void merge(BinarySearchTree<T> &tree2, BinarySearchTree<T> &merged_tree);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	size_t size() { return size_; };
	constexpr T limit() { return -1; }
	void printInOrder();
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree() {
	this->root = nullptr;
	this->size_ = 0;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
	BinarySearchTreeNode<T>* curr = this->root;
	if (curr == nullptr) return;

	std::queue<BinarySearchTreeNode<T>*> destruct;
	destruct.push(curr);

	while(!destruct.empty()) {
		curr = destruct.front();
		destruct.pop();

		if (curr->children[LEFT] != nullptr) destruct.push(curr->children[LEFT]);
		if (curr->children[RIGHT] != nullptr) destruct.push(curr->children[RIGHT]);

		delete curr;
		this->size_--;
	}
}

template <typename T>
void BinarySearchTree<T>::insert(T key) {
	if (contains(key)) return;
	
	this->size_++;
	BinarySearchTreeNode<T>** curr = &(this->root);

	while (*curr != nullptr) {
		if (key < (*curr)->key )
			curr = &(*curr)->children[LEFT];
		else
			curr = &(*curr)->children[RIGHT];
	}

	BinarySearchTreeNode<T>* temp = new BinarySearchTreeNode<T>(key);
	*curr = temp;
}

template <typename T>
void BinarySearchTree<T>::remove(T key) {
	if (!contains(key)) return;

	this->size_--;
	BinarySearchTreeNode<T>* curr = findNode(this->root, key);
	BinarySearchTreeNode<T>* parent = findParent(this->root, key);
	BinarySearchTreeNode<T>* temp = nullptr;
	BinarySearchTreeNode<T>* succ = successor_node(key);


	if (curr == this->root) {
		if (succ != nullptr) {
			curr->key = succ->key;
			curr->children[RIGHT] = succ->children[RIGHT];
			delete succ;
		}
		else {
			this->root = this->root->children[LEFT];
			delete curr;	
		}
	}

	else if (curr->children[LEFT] == nullptr || curr->children[RIGHT] == nullptr ) { // leaf node or one child
		if (curr->children[LEFT] != nullptr)
			temp = curr->children[LEFT];
		else
			temp = curr->children[RIGHT];
		
		if (curr == parent->children[LEFT])
			parent->children[LEFT] = temp;
		else
			parent->children[RIGHT] = temp;

		delete curr;
	}
	else { // two children {
		parent = findParent(curr->children[RIGHT], succ->key);
		if (parent != nullptr)
			parent->children[LEFT] = succ->children[RIGHT];
		else
			curr->children[RIGHT] = succ->children[RIGHT];
		
		curr->key = succ->key;
		delete succ;
	}
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::min(BinarySearchTreeNode<T>* curr) {
	if (this->size_ == 0) return nullptr;
	
	while (curr->children[LEFT] != nullptr) {
		curr = curr->children[LEFT];
	}
	
	return curr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::max(BinarySearchTreeNode<T>* curr) {
	if (this->size_ == 0) return nullptr;

	while (curr->children[RIGHT] != nullptr) {
		curr = curr->children[RIGHT];
	}
	
	return curr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findNode(BinarySearchTreeNode<T>* curr, T key) {
	while (curr != nullptr && curr->key != key) {
		if (curr->key > key)
			curr = curr->children[LEFT];
		else
			curr = curr->children[RIGHT];		
	}

	if (curr == nullptr) return nullptr;
	if (curr->key == key) return curr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findParent(BinarySearchTreeNode<T>* curr, T key) {
	BinarySearchTreeNode<T>* parent = nullptr;

	while (curr != nullptr && curr->key != key) {
		parent = curr;
		if (curr->key > key)
			curr = curr->children[LEFT];
		else
			curr = curr->children[RIGHT];		
	}

	if (curr == nullptr) return nullptr;
	if (curr->key == key) return parent;
}

template <typename T>
bool BinarySearchTree<T>::contains(T key) {
	if (findNode(this->root, key) == nullptr) return false;

	return true;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::predecessor_node(T key) {
	if (!contains(key)) return nullptr;
	
	BinarySearchTreeNode<T>* curr = this->root;
	BinarySearchTreeNode<T>* target = findNode(curr, key);

	if (target->children[LEFT] != nullptr) return max(target->children[LEFT]);
	else {
		BinarySearchTreeNode<T>* pred = nullptr;

		while (curr != target) {
			if (curr->key < target->key) {
				pred = curr;
				curr = curr->children[RIGHT];
			}
			else
				curr = curr->children[LEFT];
		}
		return pred;
	}
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::successor_node(T key) {
	if (!contains(key)) return nullptr;

	BinarySearchTreeNode<T>* curr = this->root;
	BinarySearchTreeNode<T>* target = findNode(curr, key);

	if (target->children[RIGHT] != nullptr)
		return min(target->children[RIGHT]);
	else {
		BinarySearchTreeNode<T>* succ = nullptr;

		while (curr != target) {
			if (curr->key > target->key) {
				succ = curr;
				curr = curr->children[LEFT];
			}
			else
				curr = curr->children[RIGHT];
		}
		return succ;
	}
}

template <typename T>
std::optional<T> BinarySearchTree<T>::predecessor(T key) {
	BinarySearchTreeNode<T>* pred = this->predecessor_node(key);
	if (pred) return std::optional<T>(pred->key);
	return std::nullopt;
}

template <typename T>
std::optional<T> BinarySearchTree<T>::successor(T key) {
	BinarySearchTreeNode<T>* succ = this->successor_node(key);
	if (succ) return std::optional<T>(succ->key);
	return std::nullopt;
}

template <typename T>
void BinarySearchTree<T>::inOrderVector(std::vector<T> &bst_data) {
	BinarySearchTreeNode<T>* curr = this->root;

	std::stack<BinarySearchTreeNode<T>*> printer;

	while(curr != nullptr || !printer.empty()) {
		while(curr != nullptr) {
			printer.push(curr);
			curr = curr->children[LEFT];
		}

		curr = printer.top();
		printer.pop();

		bst_data.push_back(curr->key);
		curr = curr->children[RIGHT];
	}
}

template <typename T> 
BinarySearchTreeNode<T>* BinarySearchTree<T>::generateBST(std::vector<T> &bst_data, int start, int end) {
	if (start > end) return nullptr;

	int mid_index = (start + end) / 2;
	BinarySearchTreeNode<T>* curr = new BinarySearchTreeNode<T>(bst_data[mid_index]);

	curr->children[LEFT] = generateBST(bst_data, start, mid_index - 1);
	curr->children[RIGHT] = generateBST(bst_data, mid_index + 1, end);

	return curr;
}

template <typename T>
void BinarySearchTree<T>::split(BinarySearchTree<T> &tree2, T key) {
	if (!contains(key)) return;

	std::vector<T> data;
	this->inOrderVector(data);
	int split_index = std::find(data.begin(), data.end(), key) - data.begin() - 1;

	this->~BinarySearchTree();

	this->root = generateBST(data, 0, split_index);
	this->size_ = split_index + 1;
	tree2.root = generateBST(data, split_index + 1, data.size() - 1);
	tree2.size_ = data.size() - split_index - 1;
}

template <typename T>
void BinarySearchTree<T>::merge(BinarySearchTree<T> &tree2, BinarySearchTree<T> &merged_tree) {
	if (this->root == nullptr && tree2.root == nullptr) return;

	std::vector<T> data;
	this->inOrderVector(data);
	tree2.inOrderVector(data);
	std::sort(data.begin(), data.end());
	
	merged_tree.root = generateBST(data, 0, data.size() - 1);
	merged_tree.size_ = data.size();
}

template <typename T>
void BinarySearchTree<T>::printInOrder() {
	BinarySearchTreeNode<T>* curr = this->root;

	std::stack<BinarySearchTreeNode<T>*> printer;

	while(curr != nullptr || !printer.empty()) {
		while(curr != nullptr) {
			printer.push(curr);
			curr = curr->children[LEFT];
		}

		curr = printer.top();
		printer.pop();
	
		std::cout << curr->key << " ";
		curr = curr->children[RIGHT];
	}
}