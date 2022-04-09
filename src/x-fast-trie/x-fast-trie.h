#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include "../constants.h"
#include <optional>
#include <vector>
#include <assert.h>
#include <type_traits>

/**
 * @brief Data structure for fast dynamic ordered set operations on a bounded universe.
 * 
 * @tparam Key_ is the key type.
 */
template <typename Key_>
class XFastTrie {
public:
	using key_type = Key_;
	static_assert(std::is_unsigned<key_type>::value, "Key type must be an unsigned integer.");

	using some_key_type = std::optional<key_type>;
	using size_type = size_t;

private:
	using node_type = XFastTrieNode<key_type>;
	using node_ptr = node_type*;
	using node_ptr_pair = std::pair<node_ptr, node_ptr>;

	using level_type = map_wrapper<key_type, node_ptr>;
	using lss_type = std::vector<level_type>;

private:
	size_type size_;
	lss_type lss_;

private:
	// The number of bits in the keys.
	static constexpr size_type bit_length_ = std::numeric_limits<key_type>::digits;

	// The maximum key value.
	static constexpr key_type upper_bound_ = std::numeric_limits<key_type>::max();

	// The minimum key value.
	static constexpr key_type lower_bound_ = std::numeric_limits<key_type>::min();

	static_assert(bit_length_ > 0, "Keys must have more than 0 bits.");
	static_assert(upper_bound_ >= 0, "Maximum possible key must be nonnegative.");
	static_assert(lower_bound_ == 0, "Minimum possible key must be 0.");

private:
	static constexpr bool left_ = 0;
	static constexpr bool right_ = 1;

private:

	/**
	 * @brief Get the prefix of a key on a given level.
	 * 
	 * @param key to get the prefix of.
	 * @param level to get the prefix on.
	 * @return the prefix of the key on the level.
	 */
	inline key_type get_prefix(key_type key, size_type level_index) const noexcept(NEX) {
		assert((level_index >= 0) && (level_index <= bit_length_));
		if (level_index == 0) return 0;
		return key >> (bit_length_ - level_index);
	}

	/**
	 * @brief Get the direction from a prefix.
	 * 
	 * @return the direction from the prefix.
	 */
	inline bool get_direction(key_type prefix) const noexcept {
		return prefix & 1;
	}

	/**
	 * @brief Get the level index of the longest matching prefix of a given key.
	 * 
	 * @param key to find the longest matching prefix with.
	 * @return level index of the longest matching prefix.
	 */
	size_type get_level_index_of_longest_matching_prefix(key_type key) const noexcept(NEX) {
		size_type low_level = 0;
		size_type high_level = bit_length_;

		// Binary search for the transition between levels 
		// containing the prefix and not containing the prefix.
		while (low_level <= high_level) {
			size_type mid_level = (low_level + high_level) >> 1;
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
	node_ptr get_closest_leaf(key_type key) const noexcept(NEX) {
		// If the key is in the bottom layer, look up the node and return.
		if (contains(key)) return lss_.at(bit_length_).at(key);

		// Otherwise, search for the longest matching prefix node.
		auto level_index = get_level_index_of_longest_matching_prefix(key);
		auto prefix = get_prefix(key, level_index);
		auto node = lss_.at(level_index).at(prefix);

		// Get the direction of the child.
		auto child_prefix = get_prefix(key, level_index + 1);
		bool direction = get_direction(child_prefix);

		// Compute the two possible leaves.
		auto leaf = (direction == right_) ? node->get_right() : node->get_left();
		auto other_leaf = (direction == right_) ? leaf->get_right() : leaf->get_left();

		// If the other leaf is nullptr, we can shortcut and return.
		if (other_leaf == nullptr)
			return leaf;

		// Otherwise, compute the distance between the key and each leaf.
		auto leaf_key = leaf->key();
		auto leaf_dist = leaf_key > key ? leaf_key - key : key - leaf_key;
		auto other_leaf_key = other_leaf->key();
		auto other_leaf_dist = other_leaf_key > key ? other_leaf_key - key : key - other_leaf_key;

		// Then, return the leaf that is closest to the key.
		if (leaf_dist < other_leaf_dist)
			return leaf;
		return other_leaf;
	}

	/**
	 * @brief Get the predecessor node of a given key.
	 * 
	 * @param key to get the predecessor node of.
	 * @return the predecessor node.
	 */
	node_ptr get_predecessor_node(key_type key) const noexcept(NEX) {
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
	node_ptr get_successor_node(key_type key) const noexcept(NEX) {
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
	node_ptr_pair get_predecessor_and_successor_nodes(key_type key) const noexcept(NEX) {
		if (empty()) return node_ptr_pair(nullptr, nullptr);
		auto node = get_closest_leaf(key);
		if (key < node->key())
			return node_ptr_pair(node->get_left(), node);
		if (key > node->key())
			return node_ptr_pair(node, node->get_right());
		return node_ptr_pair(node->get_left(), node->get_right());
	}

public:
	/**
	 * @brief Find the maximum possible key.
	 *
	 * @return key_type maximum possible key.
	 */
	static constexpr key_type upper_bound() {
		return upper_bound_;
	}

	/**
	 * @brief Find the minimum possible key.
	 * 
	 * @return key_type mimimum possible key.
	 */
	static constexpr key_type lower_bound() {
		return lower_bound_;
	}

	/**
	 * @brief Get the bit length of the keys.
	 * 
	 * @return bit length of the keys.
	 */
	static constexpr size_type bit_length() {
		return bit_length_;
	}

public:
	/**
	 * @brief Construct a new XFastTrie object.
	 * 
	 */
	XFastTrie() : size_(0) {
		lss_.reserve(bit_length_);
		for (size_type i = 0; i <= bit_length_; ++i)
			lss_.push_back(level_type());
	}

	/**
	 * @brief Get the number of keys stored in the trie.
	 * 
	 * @return size_type number of keys in stored the trie.
	 */
	inline size_type size() const noexcept {
		return size_;
	}

	/**
	 * @brief Check if the trie is empty.
	 * 
	 * @return true if the trie contains no keys.
	 * @return false if the trie contains some keys.
	 */
	inline bool empty() const noexcept {
		return size() == 0;
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	inline bool contains(key_type key) const noexcept(NEX) {
		return lss_.at(bit_length_).contains(key);
	}

	/**
	 * @brief Find the predecessor of a key.
	 * 
	 * @param key to find the predecessor of.
	 * @return some_key_type predecessor key if the predecessor exists. 
	 * @return none_key_type if the predecessor does not exist.
	 */
	some_key_type predecessor(key_type key) const noexcept(NEX) {
		auto node = get_predecessor_node(key);
		if (node != nullptr) 
			return some_key_type(node->key());
		return some_key_type();
	}

	/**
	 * @brief Find the successor of a key.
	 * 
	 * @param key to find the successor of.
	 * @return some_key_type successor key if the successor exists.
	 * @return none_key_type if the successor does not exist.
	 */
	some_key_type successor(key_type key) const noexcept(NEX) {
		auto node = get_successor_node(key);
		if (node != nullptr)
			return some_key_type(node->key());
		return some_key_type();
	}

	/**
	 * @brief Find the minimum key.
	 * 
	 * @return some_key_type minimum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type min() const noexcept(NEX) {
		if (contains(lower_bound()))
			return lower_bound();
		return successor(lower_bound());
	}

	/**
	 * @brief Find the maximum key.
	 * 
	 * @return some_key_type maximum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type max() const noexcept(NEX) {
		if (contains(upper_bound()))
			return upper_bound();
		return predecessor(upper_bound());
	}

	/**
	 * @brief Insert a key into the trie.
	 * 
	 * @param key to insert into the trie.
	 */
	void insert(key_type key) noexcept(NEX) {
		// Prevent double insertions.
		if (contains(key)) return;

		// Get the predecessor and successor nodes.
		auto pred_and_succ = get_predecessor_and_successor_nodes(key);
		auto pred = pred_and_succ.first;
		auto succ = pred_and_succ.second;

		// Create the new leaf node.
		auto leaf = new node_type(key, pred, succ);
		lss_.at(bit_length_)[key] = leaf;
		size_ += 1;

		// Insert the new leaf node into the bottom of the trie.
		if (pred != nullptr) 
			pred->set_right(leaf);

		if (succ != nullptr) 
			succ->set_left(leaf);

		// Create the root if it does not exist.
		if (!lss_.at(0).contains(0))
			lss_.at(0)[0] = new node_type(0);
		
		// Insert new internal nodes and update skip links.
		auto parent = lss_.at(0).at(0);
		for (int level_index = 1; level_index < bit_length_; ++level_index) {
			auto prefix = get_prefix(key, level_index);
			auto direction = get_direction(prefix);

			if (direction == left_) {
				// If the left pointer of the parent is nullptr or is a skip link,
				// the left internal node does not exist so we insert a new internal node.
				if (parent->get_left() == nullptr || parent->is_left_skip_link()) {
					auto node = new node_type(prefix);
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
					auto node = new node_type(prefix);
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
		if (direction == left_) {
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
	void remove(key_type key) noexcept(NEX) {
		// Prevent double removes.
		if (!contains(key)) return;

		// Get the leaf node that we are removing as well as the predecessor and successor nodes.
		auto leaf = lss_.at(bit_length_).at(key);
		auto pred = leaf->get_left();
		auto succ = leaf->get_right();

		// Remove the leaf node.
		lss_.at(bit_length_).remove(key);
		size_ -= 1;

		// Update the linked list on the bottom layer.
		if (pred != nullptr) 
			pred->set_right(succ);

		if (succ != nullptr) 
			succ->set_left(pred);

		// Remove internal nodes and update skip links.
		// TODO: Include a parent pointer in the internal nodes to avoid hash table lookups.
		for (int level = bit_length_ - 1; level >= 0; --level) {
			auto prefix = get_prefix(key, level);

			// Compute the prefix of the left and right children.
			auto left_child_prefix = prefix << 1;
			auto right_child_prefix = (prefix << 1) | 1;

			// Check if the children are in the level search structure.
			bool left_child_exists = lss_.at(level + 1).contains(left_child_prefix);
			bool right_child_exists = lss_.at(level + 1).contains(right_child_prefix);

			// If neither child has changed, do not change the parent.
			if (left_child_exists && right_child_exists)
				continue;

			// Compute the parent.
			auto parent = lss_.at(level).at(prefix);	

			// If neither child is in the level search structure, delete the parent.
			if (!left_child_exists && !right_child_exists) {
				delete parent;
				lss_.at(level).remove(prefix);
			} 

			// Otherwise, only one child has been removed, update the skip links.
			else {
				// If the left child was removed, update the skip link.
				if (!left_child_exists && (parent->get_left() == leaf || !parent->is_left_skip_link())) {
					parent->set_left_skip_link(succ);
				}
				// Otherwise, if the right child was removed, update the skip link.
				else if (!right_child_exists && (parent->get_right() == leaf || !parent->is_right_skip_link())) {
					parent->set_right_skip_link(pred);
				}
			}
		}
		delete leaf;
	}

	/**
	 * @brief Destroy the XFastTrie object.
	 * 
	 */
	~XFastTrie() {
		for (auto level : lss_)
			for (auto key_and_node : level)
				delete key_and_node.second;
	}

public:
	template <typename> friend class YFastTrie;
};