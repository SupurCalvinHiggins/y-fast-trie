#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include <optional>
#include <vector>
#include <assert.h>
#include <algorithm>

#define LEFT 0
#define RIGHT 1

template <typename T>
class XFastTrie {
private:
	const size_t limit_;
	size_t max_bits_;
	size_t size_;
	std::vector<map_wrapper<T, XFastTrieNode<T>*>> lss;
	T prefix(T key, T level);
	T get_highest_level(T key);
	T get_closest_key(T key);
	XFastTrieNode<T>* predecessor_node(T key);
	XFastTrieNode<T>* successor_node(T key);
public:
	XFastTrie();
	bool contains(T key);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	std::optional<T> min();
	std::optional<T> max();
	size_t size() { return size_; };
	const size_t limit() { return limit_; }
	void insert(T key);
	void remove(T key) {};
};

template <typename T>
XFastTrie<T>::XFastTrie() : limit_(std::numeric_limits<T>::digits) {
	size_ = 0;
	max_bits_ = std::numeric_limits<T>::digits;
	this->lss.reserve(max_bits_);
	for (size_t i = 0; i < (max_bits_ + 1); ++i) {
		this->lss.push_back(map_wrapper<T, XFastTrieNode<T>*>());
	}
}

template <typename T>
T XFastTrie<T>::prefix(T key, T level) {
	return key >> (this->max_bits_ - level);
}

template <typename T>
T XFastTrie<T>::get_highest_level(T key) {
	T low_level = 0;
	T high_level = this->max_bits_;

	// TODO: branchless?
	while (low_level <= high_level) {
		T mid_level = (low_level + high_level) >> 1;
		T prefix = this->prefix(key, mid_level);
		if (this->lss[mid_level].contains(prefix)) {
			low_level = mid_level + 1;
		}
		else {
			high_level = mid_level - 1;
		}
	}

	return low_level - 1;
}

template <typename T>
T XFastTrie<T>::get_closest_key(T key) {
	T highest_level = this->get_highest_level(key);
	T prefix = this->prefix(key, highest_level);
	XFastTrieNode<T>* node = this->lss[highest_level].at(prefix);

	if (highest_level == this->max_bits_) {
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

	if (closest_keys.size() == 0) assert(false);

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

	this->lss.back().insert(key, node);

	if (pred != nullptr) pred->children[RIGHT] = node;
	if (succ != nullptr) succ->children[LEFT] = node;

	XFastTrieNode<T>* pre = node;
	for (int level = this->max_bits_ - 1; level >= 0; --level) {
		T prefix = this->prefix(key, level);
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
			this->lss[level].insert(prefix, to_insert);
			pre = to_insert;
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
