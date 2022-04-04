#pragma once
#include <cstdint>
#include <assert.h>
#include <type_traits>

static constexpr bool ex = true;

/**
 * @brief Node structure that uses the lower bits of child pointers for control bits.
 * 
 * @tparam Key_ is the key type of the node.
 */
template <typename Key_>
class XFastTrieNode {

	using key_value = Key_;

	using node_value = XFastTrieNode<key_value>;
	using node_ptr = node_value*;

private:
	static_assert(std::is_unsigned<key_value>::value, "Key type must be an unsigned integer.");

private:

	// Mask that excludes the control bits of a pointer.
	static constexpr uintptr_t ptr_mask_ = ~static_cast<uintptr_t>(1);

	// Mask that excludes the pointer bits of a pointer.
	static constexpr uintptr_t bit_mask_ = static_cast<uintptr_t>(1);

private:
	key_value key_;

	node_ptr left_;
	node_ptr right_;

private:
	/**
	 * @brief Convert a node pointer into a clean pointer.
	 * 
	 * @param node pointer to convert.
	 * @return converted node pointer.
	 */
	inline node_ptr to_clean_ptr(node_ptr node) const noexcept(ex) {
		return reinterpret_cast<node_ptr>(
			reinterpret_cast<uintptr_t>(node) & ptr_mask_
		);
	}

	/**
	 * @brief Check if a node pointer is a clean pointer.
	 * 
	 * @param node pointer to check.
	 * @return true if the node pointer is a clean pointer.
	 * @return false if the node pointer is not a clean pointer.
	 */
	inline bool is_clean_ptr(node_ptr node) const noexcept(ex) {
		return !reinterpret_cast<node_ptr>(
			reinterpret_cast<uintptr_t>(node) & bit_mask_
		);
	}

	/**
	 * @brief Convert a node pointer into a skip link.
	 * 
	 * @param node pointer to convert.
	 * @return converted node pointer.
	 */
	inline node_ptr to_skip_link(node_ptr node) const noexcept(ex) {
		return reinterpret_cast<node_ptr>(
			reinterpret_cast<uintptr_t>(node) ^ bit_mask_
		);
	}

	/**
	 * @brief Check if a node pointer is a skip link.
	 * 
	 * @param node pointer to check.
	 * @return true if the node pointer is a skip link.
	 * @return false if the node pointer is not a skip link.
	 */
	inline bool is_skip_link(node_ptr node) const noexcept(ex) {
		return reinterpret_cast<node_ptr>(
			reinterpret_cast<uintptr_t>(node) & bit_mask_
		);
	}

public:
	/**
	 * @brief Construct a new XFastTrieNode object.
	 * 
	 * @param key is the node's key.
	 * @param left is the node's left child pointer.
	 * @param right is the node's right child pointer.
	 */
	XFastTrieNode(key_value key, node_ptr left, node_ptr right) 
		: key_(key), left_(left), right_(right) {  }

	/**
	 * @brief Construct a new XFastTrieNode object with no children.
	 * 
	 * @param key is the node's key.
	 */
	XFastTrieNode(key_value key)
		: key_(key), left_(nullptr), right_(nullptr) {  }

public:
	/**
	 * @brief Get the left child.
	 * 
	 * @return pointer to the left child. 
	 */
	inline node_ptr get_left() const noexcept(ex) {
		return to_clean_ptr(left_);
	}

	/**
	 * @brief Get the right child.
	 * 
	 * @return pointer to the right child. 
	 */
	inline node_ptr get_right() const noexcept(ex) {
		return to_clean_ptr(right_);
	}

	/**
	 * @brief Set the left child.
	 * 
	 * @param left is the new left child.
	 */
	inline void set_left(node_ptr left) noexcept(ex) {
		assert(is_clean_ptr(left));
		left_ = left;
	}

	/**
	 * @brief Set the right child.
	 * 
	 * @param right is the new right child.
	 */
	inline void set_right(node_ptr right) noexcept(ex) {
		assert(is_clean_ptr(right));
		right_ = right;
	}

	/**
	 * @brief Set the left child and mark as a skip link.
	 * 
	 * @param left is the new left child.
	 */
	inline void set_left_skip_link(node_ptr left) noexcept(ex) {
		assert(is_clean_ptr(left));
		left_ = to_skip_link(left);
	}

	/**
	 * @brief Set the right child and mark as a skip link.
	 * 
	 * @param right is the new right child.
	 */
	inline void set_right_skip_link(node_ptr right) noexcept(ex) {
		assert(is_clean_ptr(right));
		right_ = to_skip_link(right);
	}

	/**
	 * @brief Check if the left child is a skip link.
	 * 
	 * @return true if the left child is a skip link.
	 * @return false if the left child is not a skip link.
	 */
	inline bool is_left_skip_link() const noexcept(ex) {
		return is_skip_link(left_);
	}

	/**
	 * @brief Check if the right child is a skip link.
	 * 
	 * @return true if the right child is a skip link.
	 * @return false if the right child is not a skip link.
	 */
	inline bool is_right_skip_link() const noexcept(ex) {
		return is_skip_link(right_);
	}

	/**
	 * @brief Get the key of the node.
	 * 
	 * @return key of the node. 
	 */
	inline key_value key() const noexcept(ex) {
		return key_;
	}
};