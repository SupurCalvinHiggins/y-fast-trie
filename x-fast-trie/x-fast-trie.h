#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include <optional>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <string>
#include <sstream>

#define LEFT 0
#define RIGHT 1

template <typename T>
class XFastTrie {
private:
	size_t bits_;
	size_t size_;
	std::vector<map_wrapper<T, XFastTrieNode<T>*>> lss;
	T get_prefix(T key, T level);
	T get_highest_level(T key);
	T get_closest_key(T key);
	XFastTrieNode<T>* predecessor_node(T key);
	XFastTrieNode<T>* successor_node(T key);
public:
	XFastTrie();
	~XFastTrie();
	bool contains(T key);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	std::optional<T> min();
	std::optional<T> max();
	size_t size() { return size_; };
	const T limit() { return -1; }
	void insert(T key);
	void remove(T key);
};

template <typename T>
XFastTrie<T>::XFastTrie() {
	size_ = 0;
	bits_ = std::numeric_limits<T>::digits;
	this->lss.reserve(bits_);
	for (size_t i = 0; i < (bits_ + 1); ++i) {
		this->lss.push_back(map_wrapper<T, XFastTrieNode<T>*>());
	}
}

template <typename T>
XFastTrie<T>::~XFastTrie() {
	for (auto m : this->lss) {
		for (auto x : m) {
			delete x.second;
		}
	}
}

template <typename T>
T XFastTrie<T>::get_prefix(T key, T level) {
	if (level == 0) return 0;
	return key >> (this->bits_ - level);
}

template <typename T>
T XFastTrie<T>::get_highest_level(T key) {
	T low_level = 0;
	T high_level = bits_;

	while (low_level <= high_level) {
		T mid_level = low_level + ((high_level - low_level) >> 1);
		T prefix = get_prefix(key, mid_level);
		if (lss[mid_level].contains(prefix)) 
			low_level = mid_level + 1;
		else 
			high_level = mid_level - 1;
	}

	return low_level - 1;
}

template <typename T>
T XFastTrie<T>::get_closest_key(T key) {
	T highest_level = this->get_highest_level(key);
	T prefix = get_prefix(key, highest_level);
	XFastTrieNode<T>* node = this->lss[highest_level].at(prefix);

	if (highest_level == this->bits_) {
		return node->key;
	}

	std::vector<T> closest_keys;
	closest_keys.reserve(3);

	if (this->lss.back().contains(node->children[LEFT]->key) && (this->lss.back().at(node->children[LEFT]->key) == node->children[LEFT])) {
		closest_keys.push_back(node->children[LEFT]->key);
		if (node->children[LEFT]->children[LEFT]) closest_keys.push_back(node->children[LEFT]->children[LEFT]->key);
		if (node->children[LEFT]->children[RIGHT]) closest_keys.push_back(node->children[LEFT]->children[RIGHT]->key);
	}
	if (this->lss.back().contains(node->children[RIGHT]->key) && (this->lss.back().at(node->children[RIGHT]->key) == node->children[RIGHT])) {
		closest_keys.push_back(node->children[RIGHT]->key);
		if (node->children[RIGHT]->children[LEFT]) closest_keys.push_back(node->children[RIGHT]->children[LEFT]->key);
		if (node->children[RIGHT]->children[RIGHT]) closest_keys.push_back(node->children[RIGHT]->children[RIGHT]->key);
	}

	return *std::min_element(std::begin(closest_keys), std::end(closest_keys),
		[&key](T a, T b) -> T { return (a > key ? a - key : key - a) < (b > key ? b - key : key - b); });
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::predecessor_node(T key) {
	if (this->lss.back().empty()) return nullptr;

	T closest_key = this->get_closest_key(key);
	if (key <= closest_key) {
		return this->lss.back().at(closest_key)->children[LEFT];
	}
	else {
		return this->lss.back().at(closest_key);
	}
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::successor_node(T key) {
	if (this->lss.back().empty()) return nullptr;

	T closest_key = this->get_closest_key(key);
	if (key >= closest_key) {
		return this->lss.back().at(closest_key)->children[RIGHT];
	}
	else {
		return this->lss.back().at(closest_key);
	}
}

template <typename T>
void XFastTrie<T>::insert(T key) {
	if (this->lss.back().contains(key)) return;

	this->size_ += 1;

	XFastTrieNode<T>* pred = this->predecessor_node(key);
	XFastTrieNode<T>* succ = this->successor_node(key);
	XFastTrieNode<T>* children[2] = { pred, succ };
	XFastTrieNode<T>* node = new XFastTrieNode<T>(key, children);

	this->lss.back()[key] = node;

	if (pred != nullptr) pred->children[RIGHT] = node;
	if (succ != nullptr) succ->children[LEFT] = node;

	XFastTrieNode<T>* pre = node;
	for (int level = this->bits_ - 1; level >= 0; --level) {
		T prefix = get_prefix(key, level);
		if (this->lss[level].contains(prefix)) {
			XFastTrieNode<T>* cur = this->lss[level].at(prefix);

			T left_child_prefix = prefix << 1;
			T right_child_prefix = (prefix << 1) + 1;

			if (this->lss[level + 1].contains(left_child_prefix)) {
				cur->children[LEFT] = this->lss[level + 1].at(left_child_prefix);
			}
			else if (key < cur->children[LEFT]->key) {
				cur->children[LEFT] = node;
			}

			if (this->lss[level + 1].contains(right_child_prefix)) {
				cur->children[RIGHT] = this->lss[level + 1].at(right_child_prefix);
			}
			else if (key > cur->children[RIGHT]->key) {
				cur->children[RIGHT] = node;
			}
		}
		else {
			XFastTrieNode<T>* children[2] = {pre, pre};

			T is_left = pre->key & 1 ? 0 : 1;
			while (this->lss.back().contains(children[is_left]->key) ?
				this->lss.back().at(children[is_left]->key) != children[is_left] : true) {
				children[is_left] = children[is_left]->children[is_left];
			}
			XFastTrieNode<T>* to_insert = new XFastTrieNode<T>(prefix, children);
			this->lss[level][prefix] = to_insert;
			pre = to_insert;
		}		
	}
}

template <typename T>
void XFastTrie<T>::remove(T key) {
	if (!this->lss.back().contains(key)) return;

	this->size_ -= 1;

	XFastTrieNode<T>* pred = this->predecessor_node(key);
	XFastTrieNode<T>* succ = this->successor_node(key);
	XFastTrieNode<T>* node = this->lss.back().at(key);
	this->lss.back().remove(key);
	delete node;

	if (pred != nullptr) pred->children[RIGHT] = succ;
	if (succ != nullptr) succ->children[LEFT] = pred;

	for (int level = this->bits_ - 1; level >= 0; --level) {
		T prefix = get_prefix(key, level);
		T left_child_prefix = prefix << 1;
		T right_child_prefix = (prefix << 1) | 1;
		bool left_child_exists = this->lss[level + 1].contains(left_child_prefix);
		bool right_child_exists = this->lss[level + 1].contains(right_child_prefix);

		if (!left_child_exists && !right_child_exists) {
			node = this->lss[level].at(prefix);	
			delete node;
			this->lss[level].remove(prefix);
		} else {
			if (left_child_exists == true && right_child_exists == true) continue;

			T node_prefix = right_child_exists ? right_child_prefix : left_child_prefix;
			node = this->lss[level + 1].at(node_prefix);
			bool dir = right_child_exists ? LEFT : RIGHT;

			while (this->lss.back().contains(node->key) ?
				this->lss.back().at(node->key) != node : true) {
				node = node->children[dir];
			}
			
			if (right_child_exists && dir == LEFT)
				this->lss[level].at(prefix)->children[dir] = node;
			
			if (left_child_exists && dir == RIGHT)
				this->lss[level].at(prefix)->children[dir] = node;
		}
	}
}

template <typename T>
std::optional<T> XFastTrie<T>::predecessor(T key) {
	XFastTrieNode<T>* node = this->predecessor_node(key);
	if (node) return std::optional<T>(node->key);
	return std::nullopt;
}

template <typename T>
std::optional<T> XFastTrie<T>::successor(T key) {
	XFastTrieNode<T>* node = this->successor_node(key);
	if (node) return std::optional<T>(node->key);
	return std::nullopt;
}

template <typename T>
bool XFastTrie<T>::contains(T key) {
	return this->lss.back().contains(key);
}

template <typename T>
std::optional<T> XFastTrie<T>::max() {
	if (size_ == 0) return std::nullopt;
	if (this->lss.back().contains(-1)) return std::optional<T>(-1);
	XFastTrieNode<T>* node = this->predecessor_node(-1);
	return std::optional<T>(node->key);
}

template <typename T>
std::optional<T> XFastTrie<T>::min() {
	if (size_ == 0) return std::nullopt;
	if (this->lss.back().contains(0)) return std::optional<T>(0);
	XFastTrieNode<T>* node = this->successor_node(0);
	return std::optional<T>(node->key);
}