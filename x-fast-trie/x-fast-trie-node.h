#pragma once
#include "x-fast-trie.h"

template <typename T>
class XFastTrieNode {
private:
	T key;
	XFastTrieNode<T>* children[2];
public:
	XFastTrieNode(T key, XFastTrieNode<T>* children[2]);
	template <typename> friend class XFastTrie;
};

template <typename T>
XFastTrieNode<T>::XFastTrieNode(T key, XFastTrieNode<T>* children[2]) {
	this->key = key;
	this->children[0] = children[0];
	this->children[1] = children[1];
}