#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include <optional>
#include <vector>
#include <array>
#include <algorithm>
#include <assert.h>
#include <string>
#include "../y-fast-trie/y-fast-trie.h"
#include <sstream>

#define LEFT 0
#define RIGHT 1

template <typename T>
class XFastTrie {
	
	using key_value = T;
	using some_key_value = std::optional<key_value>;
	
	using node_value = XFastTrieNode<key_value>;
	using node_ptr = node_value*;
	using node_ptr_pair_value = std::pair<node_ptr, node_ptr>;

	using level_value = map_wrapper<key_value, node_ptr>;
	using lss_value = std::vector<level_value>;

private:
	size_t size_;
	lss_value lss_;

private:
	static constexpr size_t bit_length_ = std::numeric_limits<key_value>::digits;
	static constexpr key_value upper_bound_ = std::numeric_limits<key_value>::max();
	static constexpr key_value lower_bound_ = std::numeric_limits<key_value>::min();

	static_assert(bit_length_ > 0);
	static_assert(upper_bound_ >= 0);
	static_assert(lower_bound_ == 0);

private:

	/**
	 * @brief Get the prefix of a key on a given level.
	 * 
	 * @param key to get the prefix of.
	 * @param level to get the prefix on.
	 * @return the prefix of the key on the level.
	 */
	inline key_value get_prefix(key_value key, size_t level_index) const noexcept {
		if (level_index == 0) 
			return 0;
		return key >> (bit_length_ - level_index);
	}

	/**
	 * @brief Get the direction from a prefix.
	 * 
	 * @return the direction from the prefix.
	 */
	inline bool get_direction(key_value prefix) const noexcept {
		return prefix & 1;
	}

	/**
	 * @brief Get the level index of the longest matching prefix of a given key.
	 * 
	 * @param key to find the longest matching prefix with.
	 * @return level index of the longest matching prefix.
	 */
	size_t get_level_index_of_longest_matching_prefix(key_value key) {
		size_t low_level = 0;
		size_t high_level = bit_length_;

		while (low_level <= high_level) {
			size_t mid_level = (low_level + high_level) >> 1;
			auto prefix = get_prefix(key, mid_level);
			if (lss_.at(mid_level).contains(prefix)) 
				low_level = mid_level + 1;
			else 
				high_level = mid_level - 1;
		}

		return low_level - 1;
	}

	/**
	 * @brief Get the closest leaf node.
	 * 
	 * @param key to get the closest leaf to.
	 * @return closest leaf node to the key.
	 */
	node_ptr get_closest_leaf(key_value key) {
		auto level_index = get_level_index_of_longest_matching_prefix(key);
		auto prefix = get_prefix(key, level_index);
		auto node = lss_.at(level_index).at(prefix);

		// TODO: clean this terrible code up...
		if (level_index == bit_length_)
			return node;

		bool dir = get_prefix(key, level_index + 1) & 1;
		auto des = dir == RIGHT ? node->get_right() : node->get_left();

		if (des) {
			auto cand = dir == RIGHT ? des->get_right() : des->get_left();;
			if (cand == nullptr || ((cand->key() > key ? cand->key() - key : key - cand->key()) < (des->key() > key ? des->key() - key : key - des->key()))) {
				return des;
			} else {
				return cand;
			}
		}

		return des;
	}

	/**level_index
	 * @brief Get the predecessor node of a given key.
	 * 
	 * @param key to get the predecessor node of.
	 * @return the predecessor node.
	 */
	node_ptr get_predecessor_node(key_value key) {
		if (empty()) return nullptr;
		auto node = get_closest_leaf(key);
		if (key <= node->key())
			return node->get_left();
		return node;
	}

	/**
	 * @brief Get the successor node of a given key.
	 * 
	 * @param key to get the successor node of.
	 * @return the successor node.
	 */
	node_ptr get_successor_node(key_value key) {
		if (empty()) return nullptr;
		auto node = get_closest_leaf(key);
		if (key >= node->key())
			return node->get_right();
		return node;
	}

	/**
	 * @brief Get the predecessor and successor nodes of a given key.
	 * 
	 * @param key to get the predecessor and successor nodes of.
	 * @return pair containing the predecessor and successor nodes of a given key. 
	 */
	node_ptr_pair_value get_predecessor_and_successor_nodes(key_value key) {
		if (empty()) return std::make_pair(nullptr, nullptr);
		auto node = get_closest_leaf(key);
		if (key < node->key())
			return std::make_pair(node->get_left(), node);
		if (key > node->key())
			return std::make_pair(node, node->get_right());
		return std::make_pair(node->get_left(), node->get_right());
	}

public:
	XFastTrie() {
		size_ = 0;
		lss_.reserve(bit_length_);
		for (size_t i = 0; i <= bit_length_; ++i)
			lss_.push_back(level_value());
	}

	/**
	 * @brief Get the number of keys stored in the trie.
	 * 
	 * @return size_t number of keys in stored the trie.
	 */
	inline size_t size() const {
		return size_;
	}

	/**
	 * @brief Check if the trie is empty.
	 * 
	 * @return true if the trie contains no keys.
	 * @return false if the trie contains some keys.
	 */
	inline bool empty() const {
		return size() == 0;
	}

	/**
	 * @brief Find the maximum possible key.
	 *
	 * @return key_value maximum possible key.
	 */
	key_value upper_bound() {
		return upper_bound_;
	}

	/**
	 * @brief Find the minimum possible key.
	 * 
	 * @return key_value mimimum possible key.
	 */
	key_value lower_bound() {
		return lower_bound_;
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	inline bool contains(key_value key) const {
		return lss_.at(bit_length_).contains(key);
	}

	/**
	 * @brief Find the predecessor of a key.
	 * 
	 * @param key to find the predecessor of.
	 * @return some_key_value predecessor key if the predecessor exists. 
	 * @return none_key_value if the predecessor does not exist.
	 */
	some_key_value predecessor(key_value key) {
		auto node = get_predecessor_node(key);
		if (node != nullptr) 
			return some_key_value(node->key());
		return some_key_value();
	}

	/**
	 * @brief Find the successor of a key.
	 * 
	 * @param key to find the successor of.
	 * @return some_key_value successor key if the successor exists.
	 * @return none_key_value if the successor does not exist.
	 */
	some_key_value successor(key_value key) {
		auto node = get_successor_node(key);
		if (node != nullptr)
			return some_key_value(node->key());
		return some_key_value();
	}

	/**
	 * @brief Find the minimum key.
	 * 
	 * @return some_key_value minimum key if trie is not empty.
	 * @return none_key_value if the trie is empty.
	 */
	some_key_value min() {
		if (contains(lower_bound()))
			return lower_bound();
		return successor(lower_bound());
	}

	/**
	 * @brief Find the maximum key.
	 * 
	 * @return some_key_value maximum key if trie is not empty.
	 * @return none_key_value if the trie is empty.
	 */
	some_key_value max() {
		if (contains(upper_bound()))
			return upper_bound();
		return predecessor(upper_bound());
	}

	/**
	 * @brief Insert a key into the trie.
	 * 
	 * @param key to insert into the trie.
	 */
	void insert(key_value key) {
		// Prevent double insertions.
		if (contains(key)) return;

		// Get the predecessor and successor nodes.
		auto pred_and_succ = get_predecessor_and_successor_nodes(key);
		auto pred = pred_and_succ.first;
		auto succ = pred_and_succ.second;

		// Create the new leaf node.
		auto leaf = new node_value(key, pred, succ);
		lss_.at(bit_length_)[key] = leaf;
		size_ += 1;

		// Insert the new leaf node into the bottom of the trie.
		if (pred != nullptr) 
			pred->set_right(leaf);

		if (succ != nullptr) 
			succ->set_left(leaf);

		// Create the root if it does not exist.
		if (!lss_.at(0).contains(0))
			lss_.at(0)[0] = new node_value(0);
		
		// Insert new internal nodes and update skip links.
		auto parent = lss_.at(0).at(0);
		for (int level_index = 1; level_index < bit_length_; ++level_index) {
			auto prefix = get_prefix(key, level_index);
			auto direction = get_direction(prefix);

			if (direction == LEFT) {
				// If the left pointer of the parent is nullptr or is a skip link,
				// the left internal node does not exist so we insert a new internal node.
				if (parent->get_left() == nullptr || parent->is_left_skip_link()) {
					auto node = new node_value(prefix);
					lss_.at(level_index)[prefix] = node;
					parent->set_left(node);
				}

				// If the right pointer of the parent is nullptr, we must have created the parent
				// in the last step. Then, the right pointer should become a skip link to the new 
				// leaf node. Also, if the parent already has a right skip link, then we need to
				// update the right skip link if the new leaf has a lesser key, because the right
				// skip link must always point to the smallest leaf in the subtree.
				if ((parent->get_right() == nullptr) ||
				    (parent->is_right_skip_link() && key > parent->get_right()->key())) {
					parent->set_right_skip_link(leaf);
				} 

				// Set the parent to the left child because we traversed to the left.
				parent = parent->get_left();
			} 
			
			// Otherwise, the direction must be RIGHT.
			else {
				// If the right pointer of the parent is nullptr or is a skip link,
				// the right internal node does not exist so we insert a new internal node.
				if (parent->get_right() == nullptr || parent->is_right_skip_link()) {
					auto node = new node_value(prefix);
					lss_.at(level_index)[prefix] = node;
					parent->set_right(node);
				}

				// If the left pointer of the parent is nullptr, we must have created the parent
				// in the last step. Then, the left pointer should become a skip link to the new 
				// leaf node. Also, if the parent already has a left skip link, then we need to
				// update the left skip link if the new leaf has a lesser key, because the left
				// skip link must always point to the smallest leaf in the subtree.
				if ((parent->get_left() == nullptr) || 
					(parent->is_left_skip_link() && key < parent->get_left()->key())) {
					parent->set_left_skip_link(leaf);
				}

				// Set the parent to the right child because we traversed to the right.
				parent = parent->get_right();
			}
		}

		// Link the last inserted node to the leaf nodes.
		auto direction = get_direction(key);
		if (direction == LEFT) {
			parent->set_left(leaf);
			if (parent->get_right() == nullptr)
				parent->set_right(leaf);
		} 
		// Otherwise, the direction must be RIGHT.
		else {
			parent->set_right(leaf);
			if (parent->get_left() == nullptr)
				parent->set_left(leaf);
		}
	}

	/**
	 * @brief Remove a key from the trie.
	 * 
	 * @param key to remove from the trie.
	 */
	void remove(key_value key) {
		if (!contains(key)) return;

		auto pred_succ = get_predecessor_and_successor_nodes(key);
		auto pred = pred_succ.first;
		auto succ = pred_succ.second;

		auto leaf = lss_[bit_length_][key];

		lss_[bit_length_].remove(key);
		size_ -= 1;

		if (pred) pred->set_right(succ);
		if (succ) succ->set_left(pred);

		for (int level = bit_length_ - 1; level >= 0; --level) {
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
					parent->set_left_skip_link(succ);
				}
				else if (!right_child_exists && (parent->get_right() == leaf || !parent->is_right_skip_link())) {
					parent->set_right_skip_link(pred);
				}
			}
		}

		delete leaf;
	}

	~XFastTrie() {
		for (auto level : lss_)
			for (auto key_and_node : level)
				delete key_and_node.second;
	}

public:
	template <typename> friend class YFastTrie;
};