#pragma once
#include "x-fast-trie.h"

// TODO: friend class
template <typename T>
class XFastTrieNode {
public:
	XFastTrieNode(T key, XFastTrieNode<T>* children[2]);
	T key;
	XFastTrieNode<T>* children[2];
};

template <typename T>
XFastTrieNode<T>::XFastTrieNode(T key, XFastTrieNode<T>* children[2]) {
	this->key = key;
	this->children[0] = children[0];
	this->children[1] = children[1];
}