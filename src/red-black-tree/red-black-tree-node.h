/**
 * @file red-black-tree-node.h
 * @author Ethan Carlson
 * @brief This is the header for the node for RedBlackTree, an implementation of a red black tree.
 * @version 1.0
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

template <typename T> 
class Node {
public:

	using size_type = int;
	Node();
	~Node();
	Node(T key, bool color);
	Node(T key, bool color, Node* left, Node* right, Node* parent);
	Node* left();
	void left(Node* node);
	Node* right();
	void right(Node* node);
	Node* parent();
	void parent(Node* node);
	bool color();
	void color(bool c);
	T key();
	void key(T key);
	
	Node* children_[2];
	Node* parent_;
	T key_;
	//0 means black, 1 means red
	bool color_;
};

template <typename T> Node<T>::Node() {
	children_[0] = nullptr;
	children_[1] = nullptr;
	parent_ = nullptr;
	key_ = T();
	color_ = 1;
};

template <typename T> Node<T>::~Node() {

};

template <typename T> Node<T>::Node(T key, bool color) {
	children_[0] = nullptr;
	children_[1] = nullptr;
	parent_ = nullptr;
	key_ = key;
	color_ = color;
};

template <typename T> Node<T>::Node(T key, bool color, Node* left, Node* right, Node* parent) {
	children_[0] = left;
	children_[1] = right;
	parent_ = parent;
	key_ = key;
	color_ = color;
};

template <typename T> Node<T>* Node<T>::left() {
	return children_[0];
}

template <typename T> void Node<T>::left(Node* node) {
	children_[0] = node;
}

template <typename T> Node<T>* Node<T>::right() {
	return children_[1];
}

template <typename T> void Node<T>::right(Node* node) {
	children_[1] = node;
}

template <typename T> Node<T>* Node<T>::parent() {
	return parent_;
}

template <typename T> void Node<T>::parent(Node* node) {
	parent_ = node;
};

template <typename T> bool Node<T>::color() {
	return color_;
};

template <typename T> void Node<T>::color(bool c) {
	color_ = c;
};

template <typename T> T Node<T>::key() {
	return key_;
};

template <typename T> void Node<T>::key(T key) {
	key_ = key;
};