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
	std::vector<map_wrapper<T, XFastTrieNode<T>*>> lss_;
	T get_prefix(T key, size_t level);
	size_t get_highest_level(T key);
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
	void reserve(size_t size);
};

template <typename T>
XFastTrie<T>::XFastTrie() {
	size_ = 0;
	bits_ = std::numeric_limits<T>::digits;
	lss_.reserve(bits_);
	for (size_t i = 0; i < (bits_ + 1); ++i) {
		lss_.push_back(map_wrapper<T, XFastTrieNode<T>*>());
	}
}

template <typename T>
XFastTrie<T>::~XFastTrie() {
	for (auto m : lss_) {
		for (auto x : m) {
			delete x.second;
		}
	}
}

template <typename T>
T XFastTrie<T>::get_prefix(T key, size_t level) {
	if (level == 0) return 0;
	return key >> (bits_ - level);
}

template <typename T>
size_t XFastTrie<T>::get_highest_level(T key) {
	size_t low_level = 0;
	size_t high_level = bits_;

	while (low_level <= high_level) {
		size_t mid_level = low_level + ((high_level - low_level) >> 1);
		T prefix = get_prefix(key, mid_level);
		if (lss_[mid_level].contains(prefix)) 
			low_level = mid_level + 1;
		else 
			high_level = mid_level - 1;
	}

	return low_level - 1;
}

template <typename T>
T XFastTrie<T>::get_closest_key(T key) {
	T highest_level = get_highest_level(key);
	T prefix = get_prefix(key, highest_level);
	XFastTrieNode<T>* node = lss_[highest_level].at(prefix);

	if (highest_level == bits_) {
		return node->key;
	}

	std::vector<T> closest_keys;
	closest_keys.reserve(3);

	if (lss_.back().contains(node->children[LEFT]->key) && (lss_.back().at(node->children[LEFT]->key) == node->children[LEFT])) {
		closest_keys.push_back(node->children[LEFT]->key);
		if (node->children[LEFT]->children[LEFT]) closest_keys.push_back(node->children[LEFT]->children[LEFT]->key);
		if (node->children[LEFT]->children[RIGHT]) closest_keys.push_back(node->children[LEFT]->children[RIGHT]->key);
	}
	if (lss_.back().contains(node->children[RIGHT]->key) && (lss_.back().at(node->children[RIGHT]->key) == node->children[RIGHT])) {
		closest_keys.push_back(node->children[RIGHT]->key);
		if (node->children[RIGHT]->children[LEFT]) closest_keys.push_back(node->children[RIGHT]->children[LEFT]->key);
		if (node->children[RIGHT]->children[RIGHT]) closest_keys.push_back(node->children[RIGHT]->children[RIGHT]->key);
	}

	return *std::min_element(std::begin(closest_keys), std::end(closest_keys),
		[&key](T a, T b) -> T { return (a > key ? a - key : key - a) < (b > key ? b - key : key - b); });
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::predecessor_node(T key) {
	if (lss_.back().empty()) return nullptr;

	T closest_key = get_closest_key(key);
	if (key <= closest_key) {
		return lss_.back().at(closest_key)->children[LEFT];
	}
	else {
		return lss_.back().at(closest_key);
	}
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::successor_node(T key) {
	if (lss_.back().empty()) return nullptr;

	T closest_key = get_closest_key(key);
	if (key >= closest_key) {
		return lss_.back().at(closest_key)->children[RIGHT];
	}
	else {
		return lss_.back().at(closest_key);
	}
}

template <typename T>
void XFastTrie<T>::insert(T key) {
	if (lss_.back().contains(key)) return;

	size_ += 1;

	XFastTrieNode<T>* pred = predecessor_node(key);
	XFastTrieNode<T>* succ = successor_node(key);
	XFastTrieNode<T>* children[2] = { pred, succ };
	XFastTrieNode<T>* node = new XFastTrieNode<T>(key, children);

	lss_.back()[key] = node;

	if (pred != nullptr) pred->children[RIGHT] = node;
	if (succ != nullptr) succ->children[LEFT] = node;

	XFastTrieNode<T>* pre = node;
	for (int level = bits_ - 1; level >= 0; --level) {
		T prefix = get_prefix(key, level);
		if (lss_[level].contains(prefix)) {
			XFastTrieNode<T>* cur = lss_[level].at(prefix);

			T left_child_prefix = prefix << 1;
			T right_child_prefix = (prefix << 1) + 1;

			if (lss_[level + 1].contains(left_child_prefix)) {
				cur->children[LEFT] = lss_[level + 1].at(left_child_prefix);
			}
			else if (key < cur->children[LEFT]->key) {
				cur->children[LEFT] = node;
			}

			if (lss_[level + 1].contains(right_child_prefix)) {
				cur->children[RIGHT] = lss_[level + 1].at(right_child_prefix);
			}
			else if (key > cur->children[RIGHT]->key) {
				cur->children[RIGHT] = node;
			}
		}
		else {
			XFastTrieNode<T>* children[2] = {pre, pre};
			T is_left = pre->key & 1 ? 0 : 1;
			children[is_left] = node;
			XFastTrieNode<T>* to_insert = new XFastTrieNode<T>(prefix, children);
			lss_[level][prefix] = to_insert;
			pre = to_insert;
		}		
	}
}

template <typename T>
void XFastTrie<T>::remove(T key) {
	if (!lss_.back().contains(key)) return;

	size_ -= 1;

	XFastTrieNode<T>* pred = predecessor_node(key);
	XFastTrieNode<T>* succ = successor_node(key);
	XFastTrieNode<T>* end_node = lss_.back().at(key);
	lss_.back().remove(key);

	if (pred != nullptr) pred->children[RIGHT] = succ;
	if (succ != nullptr) succ->children[LEFT] = pred;

	for (int level = bits_ - 1; level >= 0; --level) {
		T prefix = get_prefix(key, level);
		T left_child_prefix = prefix << 1;
		T right_child_prefix = (prefix << 1) | 1;
		bool left_child_exists = lss_[level + 1].contains(left_child_prefix);
		bool right_child_exists = lss_[level + 1].contains(right_child_prefix);

		if (!left_child_exists && !right_child_exists) {
			auto node = lss_[level].at(prefix);	
			delete node;
			lss_[level].remove(prefix);
		} else {
			if (left_child_exists == true && right_child_exists == true) continue;

			T node_prefix = right_child_exists ? right_child_prefix : left_child_prefix;
			auto node = lss_[level + 1].at(node_prefix);
			bool dir = right_child_exists ? LEFT : RIGHT;

			// while (lss_.back().contains(node->key) ?
			// 	lss_.back().at(node->key) != node : true) {
			// 	node = node->children[dir];
			// }
			
			auto child = lss_[level].at(prefix)->children[dir];
			if (right_child_exists && !(lss_.back().contains(child->key) && lss_.back().at(child->key) == child)) {
				// assert(node == succ || node == lss_[level].at(prefix)->children[dir]);
				// if (node == lss_[level].at(prefix)->children[dir]) {
				// 	if (lss_.back().contains(child->key)) {
				// 		assert(child == lss_.back().at(child->key));
				// 	} else {
				// 		assert(false);
				// 	}
				// }
				lss_[level].at(prefix)->children[dir] = succ;
			}
			
			if (left_child_exists && !(lss_.back().contains(child->key) && lss_.back().at(child->key) == child)) {
				// assert(node == pred || node == lss_[level].at(prefix)->children[dir]);
				// if (node == lss_[level].at(prefix)->children[dir]) {
				// 	auto child = lss_[level].at(prefix)->children[dir];
				// 	if (lss_.back().contains(child->key)) {
				// 		assert(child == lss_.back().at(child->key));
				// 	} else {
				// 		assert(false);
				// 	}
				// }
				lss_[level].at(prefix)->children[dir] = pred;
			}

			// auto cur = lss_[level].at(prefix);
			// if (cur->children[LEFT] == end_node)
			// 	cur->children[LEFT] = succ;
			// else if (cur->children[RIGHT] == end_node)
			// 	cur->children[RIGHT] = pred;
			
		}
	}

	delete end_node;
}

template <typename T>
std::optional<T> XFastTrie<T>::predecessor(T key) {
	XFastTrieNode<T>* node = predecessor_node(key);
	if (node) return std::optional<T>(node->key);
	return std::nullopt;
}

template <typename T>
std::optional<T> XFastTrie<T>::successor(T key) {
	XFastTrieNode<T>* node = successor_node(key);
	if (node) return std::optional<T>(node->key);
	return std::nullopt;
}

template <typename T>
bool XFastTrie<T>::contains(T key) {
	return lss_.back().contains(key);
}

template <typename T>
std::optional<T> XFastTrie<T>::max() {
	if (size_ == 0) return std::nullopt;
	if (lss_.back().contains(-1)) return std::optional<T>(-1);
	XFastTrieNode<T>* node = predecessor_node(-1);
	return std::optional<T>(node->key);
}

template <typename T>
std::optional<T> XFastTrie<T>::min() {
	if (size_ == 0) return std::nullopt;
	if (lss_.back().contains(0)) return std::optional<T>(0);
	XFastTrieNode<T>* node = successor_node(0);
	return std::optional<T>(node->key);
}

template <typename T>
void XFastTrie<T>::reserve(size_t size) {
	for (size_t level = 0; level <= bits_; ++level) {
		lss_[level].reserve(get_prefix(size, level));
	}
}