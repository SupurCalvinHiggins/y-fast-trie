/**
 * @file x-fast-trie-node.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief XFastTrieNode class template.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include "../constants.h"
#include "../augmented-pointer/augmented-pointer.h"
#include <cstdint>
#include <assert.h>
#include <type_traits>

/**
 * @brief Node structure that uses the lower bits of child pointers for control bits.
 * 
 * @tparam Key_ is the key type of the node.
 */
template <typename Key_>
class XFastTrieNode {
public:
	using key_type = Key_;
	static_assert(std::is_unsigned<key_type>::value, "Key type must be an unsigned integer.");

private:
	using node_type = XFastTrieNode<key_type>;
	using node_ptr = node_type*;
	using aug_ptr = AugmentedPointer<node_ptr, 1>;

private:
	key_type key_;

	aug_ptr left_;
	aug_ptr right_;

public:
	/**
	 * @brief Construct a new XFastTrieNode object.
	 * 
	 * @param key is the node's key.
	 * @param left is the node's left child pointer.
	 * @param right is the node's right child pointer.
	 */
	XFastTrieNode(key_type key, node_ptr left, node_ptr right) 
		: key_(key), left_(left), right_(right) {  }

	/**
	 * @brief Construct a new XFastTrieNode object with no children.
	 * 
	 * @param key is the node's key.
	 */
	XFastTrieNode(key_type key)
		: key_(key), left_(nullptr), right_(nullptr) {  }

public:
	/**
	 * @brief Get the left child.
	 * 
	 * @return pointer to the left child. 
	 */
	inline node_ptr get_left() const noexcept(NEX) {
		return left_.get_ptr();
	}

	/**
	 * @brief Get the right child.
	 * 
	 * @return pointer to the right child. 
	 */
	inline node_ptr get_right() const noexcept(NEX) {
		return right_.get_ptr();
	}

	/**
	 * @brief Set the left child.
	 * 
	 * @param left is the new left child.
	 */
	inline void set_left(node_ptr left) noexcept(NEX) {
		left_.set_ptr(left);
	}

	/**
	 * @brief Set the right child.
	 * 
	 * @param right is the new right child.
	 */
	inline void set_right(node_ptr right) noexcept(NEX) {
		right_.set_ptr(right);
	}

	/**
	 * @brief Set the left child and mark as a skip link.
	 * 
	 * @param left is the new left child.
	 */
	inline void set_left_skip_link(node_ptr left) noexcept(NEX) {
		left_.set_ptr(left);
		left_.set_bit(0);
	}

	/**
	 * @brief Set the right child and mark as a skip link.
	 * 
	 * @param right is the new right child.
	 */
	inline void set_right_skip_link(node_ptr right) noexcept(NEX) {
		right_.set_ptr(right);
		right_.set_bit(0);
	}

	/**
	 * @brief Check if the left child is a skip link.
	 * 
	 * @return true if the left child is a skip link.
	 * @return false if the left child is not a skip link.
	 */
	inline bool is_left_skip_link() const noexcept(NEX) {
		return left_.is_set_bit(0);
	}

	/**
	 * @brief Check if the right child is a skip link.
	 * 
	 * @return true if the right child is a skip link.
	 * @return false if the right child is not a skip link.
	 */
	inline bool is_right_skip_link() const noexcept(NEX) {
		return right_.is_set_bit(0);
	}

	/**
	 * @brief Get the key of the node.
	 * 
	 * @return key of the node. 
	 */
	inline key_type key() const noexcept(NEX) {
		return key_;
	}
};