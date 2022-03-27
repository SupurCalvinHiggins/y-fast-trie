#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include <optional>
#include <vector>
#include <array>
#include <algorithm>
#include <assert.h>
#include <string>
#include <sstream>

#define LEFT 0
#define RIGHT 1

template <typename T>
class XFastTrie {
private:
	size_t size_;
	std::vector<map_wrapper<T, XFastTrieNode<T>*>> lss_;
	T get_prefix(T key, size_t level);
	size_t get_highest_level(T key);
	T get_closest_key(T key);
	XFastTrieNode<T>* predecessor_node(T key);
	XFastTrieNode<T>* successor_node(T key);
	constexpr size_t bits() { return std::numeric_limits<T>::digits; }
public:
	XFastTrie();
	~XFastTrie();
	bool contains(T key);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	std::optional<T> min();
	std::optional<T> max();
	size_t size() { return size_; };
 	constexpr T limit() { return -1; }
	void insert(T key);
	void remove(T key);
	void reserve(size_t size);
	bool empty();
	std::string ptstr(void* x) {
		std::ostringstream ss;
		ss << x;
		return std::string(ss.str());
	}
	std::string to_dot() {
	std::vector<std::string> levels;
	std::string ss;
	ss += "digraph {\n";
	for (auto m : lss_) {
		std::string level("{rank = same; ");
		// std::vector<T> data;
		// for (auto x : m) {
		// 	data.push_back(x.first);
		// }
		// std::sort(data.begin(), data.end(), std::greater<T>());
		for (auto x : m) {
			if (x.second->get_left() != nullptr) ss +="\tn" + ptstr((void*)(x.second)) + " -> n" + ptstr((void*)(x.second->get_left())) + "\n";
			if (x.second->get_right() != nullptr) ss +="\tn" + ptstr((void*)(x.second)) + " -> n" + ptstr((void*)(x.second->get_right())) + "\n";
			ss += "\tn" + ptstr((void*)(x.second)) + " [label=\"" + std::to_string(int(x.first)) + "\"]\n";
			level += "n" + ptstr((void*)(x.second)) + "; ";
			
		}
		levels.push_back(level + "}");
	}
	for (auto s : levels) {
		ss += "\t" + s + "\n"; 
	}
	ss += "}";
	return std::string(ss);
}
};

template <typename T>
XFastTrie<T>::XFastTrie() {
	size_ = 0;
	lss_.reserve(bits());
	for (size_t i = 0; i <= bits(); ++i) {
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
	return key >> (bits() - level);
}

template <typename T>
size_t XFastTrie<T>::get_highest_level(T key) {
	size_t low_level = 0;
	size_t high_level = bits();

	while (low_level <= high_level) {
		size_t mid_level = (low_level + high_level) >> 1;
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

	if (highest_level == bits()) {
		return node->key();
	}

	bool dir = get_prefix(key, highest_level + 1) & 1;
	auto des = dir == RIGHT ? node->get_right() : node->get_left();

	if (des) {
		auto cand = dir == RIGHT ? des->get_right() : des->get_left();;
		if (cand == nullptr || ((cand->key() > key ? cand->key() - key : key - cand->key()) < (des->key() > key ? des->key() - key : key - des->key()))) {
			return des->key();
		} else {
			return cand->key();
		}
	}

	return des->key();
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::predecessor_node(T key) {
	if (empty()) return nullptr;

	T closest_key = get_closest_key(key);
	auto node = lss_[bits()][closest_key];
	if (key <= closest_key)
		return node->get_left();
	return node;
}

template <typename T>
XFastTrieNode<T>* XFastTrie<T>::successor_node(T key) {
	if (empty()) return nullptr;
	T closest_key = get_closest_key(key);
	auto node = lss_[bits()][closest_key];
	if (key >= closest_key)
		return node->get_right();
	return node;
}

template <typename T>
void XFastTrie<T>::insert(T key) {
	if (contains(key)) return;

	auto pred = predecessor_node(key);
	auto succ = successor_node(key);
	auto leaf = new XFastTrieNode<T>(key, pred, succ);

	lss_[bits()][key] = leaf;
	size_ += 1;

	if (pred) pred->set_right(leaf);
	if (succ) succ->set_left(leaf);

	// create root if the root does not exist (maybe we need to persist it)
	if (!lss_[0].contains(0)) {
		lss_[0][0] = new XFastTrieNode<T>(0, nullptr, nullptr);
	}

	// WE ONLY UPDATE SKIP LINKS THAT FALL ON THE OTHER SIDE OF AN EMPTY...
	auto parent = lss_[0][0];
	for (int level = 1; level < bits(); ++level) {
		// need to properly init side
		auto prefix = get_prefix(key, level);
		auto side = prefix & 1;
		if (side == LEFT) {
			// insert node
			if (parent->get_left() == nullptr || parent->is_left_skip_link()) {
				auto node = new XFastTrieNode<T>(prefix, nullptr, nullptr);
				lss_[level][prefix] = node;
				parent->set_left(node);
			}
			// update skip link
			if (parent->get_right() == nullptr) {
				parent->set_right_skip_link(leaf);
			} else if (parent->is_right_skip_link() && key > parent->get_right()->key()) {
				parent->set_right_skip_link(leaf);
			}
			parent = parent->get_left();
		} else {
			// insert node 
			if (parent->get_right() == nullptr || parent->is_right_skip_link()) {
				auto node = new XFastTrieNode<T>(prefix, nullptr, nullptr);
				lss_[level][prefix] = node;
				parent->set_right(node);
			}
			// update skip link
			if (parent->get_left() == nullptr) {
				parent->set_left_skip_link(leaf);
			} else if (parent->is_left_skip_link() && key < parent->get_left()->key()) {
				parent->set_left_skip_link(leaf);
			}
			parent = parent->get_right();
		}
	}

	// handle linking last insert to the leaf
	// this does not work we need to discriminate between skip and not
	// to properly update the next to last node...
	if ((key & 1) == LEFT) {
		parent->set_left(leaf);
		if (parent->get_right() == nullptr) {
			parent->set_right(leaf);
		}
	} else {
		parent->set_right(leaf);
		if (parent->get_left() == nullptr) {
			parent->set_left(leaf);
		}
	}

	assert(parent->get_left()->key() <= parent->get_right()->key());
}

template <typename T>
void XFastTrie<T>::remove(T key) {
	if (!contains(key)) return;

	auto pred = predecessor_node(key);
	auto succ = successor_node(key);
	auto leaf = lss_[bits()][key];

	lss_[bits()].remove(key);
	size_ -= 1;

	if (pred) pred->set_right(succ);
	if (succ) succ->set_left(pred);

	for (int level = bits() - 1; level >= 0; --level) {
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

			auto parent = lss_[level].at(prefix);

			if (!left_child_exists && (parent->get_left() == leaf || !parent->is_left_skip_link())) {
				// if (!parent->is_left_skip_link() && level != (bits() - 1)) 
				// 	delete parent->get_left();
				parent->set_left_skip_link(succ);
			}
			else if (!right_child_exists && (parent->get_right() == leaf || !parent->is_right_skip_link())) {
				// if (!parent->is_right_skip_link() && level != (bits() - 1)) 
				// 	delete parent->get_right();
				parent->set_right_skip_link(pred);
			}
		}
	}

	delete leaf;
}

template <typename T>
std::optional<T> XFastTrie<T>::predecessor(T key) {
	XFastTrieNode<T>* node = predecessor_node(key);
	if (node) return std::optional<T>(node->key());
	return std::nullopt;
}

template <typename T>
std::optional<T> XFastTrie<T>::successor(T key) {
	XFastTrieNode<T>* node = successor_node(key);
	if (node) return std::optional<T>(node->key());
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
	return std::optional<T>(node->key());
}

template <typename T>
std::optional<T> XFastTrie<T>::min() {
	if (size_ == 0) return std::nullopt;
	if (lss_.back().contains(0)) return std::optional<T>(0);
	XFastTrieNode<T>* node = successor_node(0);
	return std::optional<T>(node->key());
}

template <typename T>
void XFastTrie<T>::reserve(size_t size) {
	for (size_t level = 0; level <= bits(); ++level) {
		lss_[level].reserve(get_prefix(size, level));
	}
}

template <typename T>
bool XFastTrie<T>::empty() {
	return size_ == 0;
}