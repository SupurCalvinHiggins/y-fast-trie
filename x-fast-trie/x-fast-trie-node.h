#pragma once
#include "x-fast-trie.h"
#include <cstdint>
#include <assert.h>


template <typename T>
class XFastTrieNode {
private:
	T key_;
	XFastTrieNode<T>* left_;
	XFastTrieNode<T>* right_;
public:
	XFastTrieNode(T key, XFastTrieNode<T>* left, XFastTrieNode<T>* right) {
		key_ = key;
		left_ = left;
		right_ = right;
	}

	XFastTrieNode(T key) {
		key_ = key;
		left_ = nullptr;
		right_ = nullptr;
	}

	static constexpr uintptr_t ptr_mask = ~static_cast<uintptr_t>(1);
	static constexpr uintptr_t bit_mask = static_cast<uintptr_t>(1);

	XFastTrieNode<T>* get_left() const {
		return reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(left_) & ptr_mask
		);
	}

	XFastTrieNode<T>* get_right() const {
		return reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(right_) & ptr_mask
		);
	}

	void set_left(XFastTrieNode<T>* left) {
		left_ = left;
	}

	void set_right(XFastTrieNode<T>* right) {
		right_ = right;
	}

	void set_left_skip_link(XFastTrieNode<T>* left) {
		assert(left != nullptr);
		left_ = reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(left) ^ bit_mask
		);
	}

	void set_right_skip_link(XFastTrieNode<T>* right) {
		assert(right != nullptr);
		right_ = reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(right) ^ bit_mask
		);
	}

	bool is_left_skip_link() const {
		return reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(left_) & bit_mask
		);
	}

	bool is_right_skip_link() const {
		return reinterpret_cast<XFastTrieNode<T>*>(
			reinterpret_cast<uintptr_t>(right_) & bit_mask
		);
	}

	T key() {
		return key_;
	}
};