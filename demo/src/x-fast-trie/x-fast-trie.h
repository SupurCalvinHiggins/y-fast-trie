#pragma once
#include "x-fast-trie-node.h"
#include "x-fast-trie-map-wrapper.h"
#include "../constants.h"
#include <optional>
#include <vector>
#include <assert.h>
#include <type_traits>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "../../gui.h"

void UPDATE_GUI();

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

protected:
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
	inline key_type get_prefix(key_type key, size_type level_index)  noexcept(NEX) {
		assert((level_index >= 0) && (level_index <= bit_length()));
		if (level_index == 0) return 0;
		return key >> (bit_length() - level_index);
	}

	/**
	 * @brief Get the direction from a prefix.
	 * 
	 * @return the direction from the prefix.
	 */
	inline bool get_direction(key_type prefix)  noexcept {
		return prefix & 1;
	}

	/**
	 * @brief Get the level index of the longest matching prefix of a given key.
	 * 
	 * @param key to find the longest matching prefix with.
	 * @return level index of the longest matching prefix.
	 */
	size_type get_lmp_level(key_type key)  noexcept(NEX) {
		size_type low_level = 0;
		size_type high_level = bit_length();

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
	 * @brief Get a leaf that is close to a given key. 
	 * 
	 * @param key to find a close leaf to.
	 * @return the close leaf.
	 */
	node_ptr get_close_leaf(key_type key)  noexcept(NEX) {
		// Get the node with the longest matching prefix.
		auto lmp_level = get_lmp_level(key);
		auto prefix = get_prefix(key, lmp_level);
		auto lmp_node = lss_.at(lmp_level).at(prefix);
		MARK_AND_UPDATE(lmp_node);

		// If the node is an internal node, we need to traverse down the skip link.
		// Otherwise, the node is already a leaf node so we do nothing.
		if (lmp_node->is_left_skip_link())
			lmp_node = lmp_node->get_left();
		else if (lmp_node->is_right_skip_link())
			lmp_node = lmp_node->get_right();

		MARK_AND_UPDATE(lmp_node);

		return lmp_node;
	}

	/**
	 * @brief Get the predecessor node of a given key.
	 * 
	 * @param key to get the predecessor node of.
	 * @return the predecessor node.
	 */
	node_ptr get_predecessor_node(key_type key)  noexcept(NEX) {
		if (empty()) return nullptr;
		auto node = get_close_leaf(key);
		if (key <= node->key()) {
			MARK_AND_UPDATE(node->get_left());
			return node->get_left();
		}
					MARK_AND_UPDATE(node);

		return node;
	}

	/**
	 * @brief Get the successor node of a given key.
	 * 
	 * @param key to get the successor node of.
	 * @return the successor node.
	 */
	node_ptr get_successor_node(key_type key)  noexcept(NEX) {
		if (empty()) return nullptr;
		auto node = get_close_leaf(key);
		if (key >= node->key()) {
			MARK_AND_UPDATE(node->get_right());
			return node->get_right();
		}
					MARK_AND_UPDATE(node);

		return node;
	}

	/**
	 * @brief Get the inclusive successor node of a given key.
	 * 
	 * @param key to get the successor node of.
	 * @return the successor node.
	 */
	node_ptr get_inclusive_successor_node(key_type key)  noexcept(NEX) {
		if (empty()) return nullptr;
		auto node = get_close_leaf(key);
		if (key > node->key()) {
			MARK_AND_UPDATE(node->get_right());
			return node->get_right();
		}
					MARK_AND_UPDATE(node);

		return node;
	}

	/**
	 * @brief Get the predecessor and successor nodes of a given key.
	 * 
	 * @param key to get the predecessor and successor nodes of.
	 * @return pair containing the predecessor and successor nodes of a given key. 
	 */
	node_ptr_pair get_predecessor_and_successor_nodes(key_type key)  noexcept(NEX) {
		if (empty()) return node_ptr_pair(nullptr, nullptr);
		auto node = get_close_leaf(key);
		if (key < node->key())
			return node_ptr_pair(node->get_left(), node);
		if (key > node->key())
			return node_ptr_pair(node, node->get_right());
		return node_ptr_pair(node->get_left(), node->get_right());
	}

	/**
	 * @brief Get leaf node from a given key.
	 * 
	 * @param key to get the leaf node of.
	 * @return the leaf node.
	 */
	node_ptr get_leaf_node(key_type key)  noexcept {
		return lss_.back().at(key);
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
		lss_.reserve(bit_length());
		for (size_type i = 0; i <= bit_length(); ++i)
			lss_.push_back(level_type());
	}

	/**
	 * @brief Get the number of keys stored in the trie.
	 * 
	 * @return size_type number of keys in stored the trie.
	 */
	inline size_type size()  noexcept {
		return size_;
	}

	/**
	 * @brief Check if the trie is empty.
	 * 
	 * @return true if the trie contains no keys.
	 * @return false if the trie contains some keys.
	 */
	inline bool empty()  noexcept {
		return size() == 0;
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	inline bool contains(key_type key)  noexcept(NEX) {
		if (lss_.at(bit_length()).contains(key)) {
			MARK_AND_UPDATE(lss_.at(bit_length()).at(key));
			return true;
		}
		return false;
	}

	/**
	 * @brief Find the predecessor of a key.
	 * 
	 * @param key to find the predecessor of.
	 * @return some_key_type predecessor key if the predecessor exists. 
	 * @return none_key_type if the predecessor does not exist.
	 */
	some_key_type predecessor(key_type key)  noexcept(NEX) {
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
	some_key_type successor(key_type key)  noexcept(NEX) {
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
	some_key_type min()  noexcept(NEX) {
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
	some_key_type max()  noexcept(NEX) {
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
		MARK_AND_UPDATE(pred);
		MARK_AND_UPDATE(succ);

		// Create the new leaf node.
		auto leaf = new node_type(key, pred, succ);
		lss_.at(bit_length())[key] = leaf;
		size_ += 1;

		// Insert the new leaf node into the bottom of the trie.
		if (pred != nullptr) 
			pred->set_right(leaf);

		if (succ != nullptr) 
			succ->set_left(leaf);

			MARK_AND_UPDATE(leaf);


		// Create the root if it does not exist.
		if (!lss_.at(0).contains(0))
			lss_.at(0)[0] = new node_type(0);
		
		// Insert new internal nodes and update skip links.
		auto parent = lss_.at(0).at(0);
			MARK_AND_UPDATE(parent);

		for (int level_index = 1; level_index < bit_length(); ++level_index) {
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
						MARK_AND_UPDATE(parent);

		}

		// Link the last inserted node to the leaf nodes.
		auto direction = get_direction(key);
		if (direction == left_) {
			parent->set_left(leaf);
			if (parent->get_right() == nullptr)
				parent->set_right_skip_link(leaf);
		} 
		// Otherwise, the direction must be RIGHT.
		else {
			parent->set_right(leaf);
			if (parent->get_left() == nullptr)
				parent->set_left_skip_link(leaf);
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
		auto leaf = lss_.at(bit_length()).at(key);
		auto pred = leaf->get_left();
		auto succ = leaf->get_right();

		// Remove the leaf node.
		lss_.at(bit_length()).remove(key);
		size_ -= 1;
		MARK_AND_UPDATE(leaf);

		// Update the linked list on the bottom layer.
		if (pred != nullptr) 
			pred->set_right(succ);

		if (succ != nullptr) 
			succ->set_left(pred);

		// Remove internal nodes and update skip links.
		// TODO: Include a parent pointer in the internal nodes to avoid hash table lookups.
		for (int level = bit_length() - 1; level >= 0; --level) {
			auto prefix = get_prefix(key, level);

			// Compute the prefix of the left and right children.
			auto left_child_prefix = prefix << 1;
			auto right_child_prefix = (prefix << 1) | 1;

			// Check if the children are in the level search structure.
			bool left_child_exists = lss_.at(level + 1).contains(left_child_prefix);
			bool right_child_exists = lss_.at(level + 1).contains(right_child_prefix);

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
			MARK_AND_UPDATE(parent);	

		}
		delete leaf;
	}

	/**
	 * @brief Destroy the XFastTrie object.
	 * 
	 */
	virtual ~XFastTrie() {
		for (auto level : lss_)
			for (auto key_and_node : level)
				delete key_and_node.second;
	}

public:
	template <typename, typename> friend class YFastTrie;

private:
    // Extra private members for the demo program.

    std::vector<node_ptr> marked_;
	bool animate_;

private:
	// Extra private methods for the demo program.

	/**
	 * @brief Converts a pointer into a DOT string.
	 * 
	 * @param ptr to convert. 
	 * @return DOT string of the pointer.
	 */
    std::string ptr_to_str(void* ptr)  noexcept {
        std::ostringstream ss;
        ss << ptr;
        return "n" + ss.str();
    }

	/**
	 * @brief Converts a key into a DOT string.
	 * 
	 * @param key to convert
	 * @return DOT string of the key.
	 */
    std::string key_to_str(key_type key)  noexcept {
        std::ostringstream ss;
        ss << int(key);
        return ss.str();
    }

    void MARK_AND_UPDATE(node_ptr ptr, std::string debug = "") {
        // if (debug != "")
        //     std::cout << "*** DEBUG ***\n" <<  debug << "\n\n";
        MARK(ptr);
		if (animate_)
        	UPDATE_GUI();
    }

    void MARK(node_ptr ptr) {
		if (animate_)
       		marked_.push_back(ptr);
    }

    void CLEAN() {
        // std::cout << "clean";
        marked_.clear();
		if (animate_)
        	UPDATE_GUI();
    }

	void CLEAN_NO_UPDATE() {
        // std::cout << "clean";
        marked_.clear();
    }

public:
	// Extra public methods for the demo program.

	void set_animate(bool animate) {
		animate_ = animate;
	}

	/**
	 * @brief Get the DOT string representation of the trie.
	 * 
	 * @return DOT string of the trie.
	 */
    std::string to_dot()  noexcept {
        std::string output;
        output += "digraph {\n";
        output += "\tnode[style=filled, fillcolor=white];\n";

        for ( auto& level : this->lss_) {
            std::string level_output;
            level_output += "{rank = same; ";

            for (auto& key_node_pair : level) {
                auto key = key_node_pair.first;
                auto node = key_node_pair.second;
                if (node->get_left()) 
                    output += "\t" + ptr_to_str(node) + " -> " + ptr_to_str(node->get_left()) + "\n";
                if (node->get_right()) 
                    output += "\t" + ptr_to_str(node) + " -> " + ptr_to_str(node->get_right()) + "\n";    
                output += "\t" + ptr_to_str(node) + "[label=\"" + key_to_str(key) + "\"]";
                if (std::find(std::begin(marked_), std::end(marked_), node) != std::end(marked_))
                    output += "[fillcolor=\"#90ee90\"]";
                output += "\n";
                level_output += ptr_to_str(node) + "; ";
            }

            level_output += "}";
            output += "\t" + level_output + "\n"; 
        }
        output += "}";
        return output;
    }
};