#pragma once 
#include "../x-fast-trie/x-fast-trie.h"
#include "../x-fast-trie/x-fast-trie-map-wrapper.h"
#include "../red-black-tree/red-black-tree.h"
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
class YFastTrie {

	using key_value = Key_;
	using some_key_value = std::optional<key_value>;

	using index_value = XFastTrie<key_value>;
	using node_ptr = typename index_value::node_ptr;

	using partition_value = RedBlackTree<key_value>;
	using partition_ptr = partition_value*;
	using partition_ptr_pair_value = std::pair<partition_ptr, partition_ptr>;

	using partition_and_node_ptr_pair_value = std::pair<partition_ptr, node_ptr>;

	using partition_index_value = map_wrapper<key_value, partition_ptr>;

private:
	static_assert(std::is_unsigned<key_value>::value, "Key type must be an unsigned integer.");

private:
	index_value index_;
	partition_index_value partitions_; 
	size_t size_;

private:
	// TODO: Should depend on the X-Fast Trie bit_length()
	// TODO: Also, consider the methods invoking such X-Fast Trie methods.
	// Mask for computing representatives.
	static constexpr key_value partition_mask_ = std::numeric_limits<key_value>::digits - 1;
	
	// Maximum partition size.
	static constexpr key_value max_partition_size_ = std::numeric_limits<key_value>::digits * 2;
	
	// Minimum partition size.
	static constexpr key_value min_partition_size_ = std::numeric_limits<key_value>::digits / 2;

private:

	/**
	 * @brief Get the representative key from a given key.
	 * 
	 * @param key to get the representative of.
	 * @return representative of the key.
	 */
	inline key_value get_representative(key_value key) const noexcept {
		return (key & ~partition_mask_) + partition_mask_;
	}

	/**
	 * @brief Get the partition and representative node of a key.
	 * 
	 * @param key to get the partition and representative node of.
	 * @return the partition and representative node.
	 */
	partition_and_node_ptr_pair_value get_partition_and_node(key_value key) const noexcept {
		// Since the index searches for strict successors, we need to subtract 1. However, we do not
		// need to subtract 1 from 0 since the representative keys are strictly greater than zero.
		auto pred_key = key == 0 ? 0 : key - 1;
		auto node = index_.get_successor_node(pred_key);

		// Now that we have computed the representative node, we can compute the partition that
		// the representative node belongs to. If the node does not exist, then the partition also
		// does not exist.
		if (node == nullptr)
			return partition_and_node_ptr_pair_value(nullptr, nullptr);
		
		// Compute the partition.
		auto partition = partitions_.at(node->key());

		return partition_and_node_ptr_pair_value(partition, node);
	}

	/**
	 * @brief Create the partition and representative node of a key if they do not exist.
	 * 
	 * @param key to create the partition and representative node of.
	 * @return the partition and representative node.
	 */
	partition_and_node_ptr_pair_value create_partition_and_node(key_value key) noexcept {
		// Since the index searches for strict successors, we need to subtract 1. However, we do not
		// need to subtract 1 from 0 since the representative keys are strictly greater than zero.
		auto pred_key = key == 0 ? 0 : key - 1;
		auto node = index_.get_successor_node(pred_key);

		// Now that we have computed the representative node, we can compute the partition that
		// the representative node belongs to. If the node does not exist, then the partition also
		// does not exist and we must create them. Otherwise, we may return as normal.
		if (node == nullptr) {
			// Insert the representative key into the index.
			auto rep_key = get_representative(key);
			index_.insert(rep_key);
			// Compute the representative node.
			node = index_.get_successor_node(rep_key - 1);
			// Create the new partition.
			partitions_[rep_key] = new partition_value();
		}

		// Compute the partition.
		auto partition = partitions_.at(node->key());

		return partition_and_node_ptr_pair_value(partition, node);
	}

public:
	/**
	 * @brief Construct a new YFastTrie object.
	 * 
	 */
    YFastTrie() : size_(0) {  }

	/**
	 * @brief Get the number of keys stored in the trie.
	 * 
	 * @return size_t number of keys in stored the trie.
	 */
	inline size_t size() const noexcept {
		return size_;
	}

	/**
	 * @brief Check if the trie is empty.
	 * 
	 * @return true if the trie contains no keys.
	 * @return false if the trie contains some keys.
	 */
	inline bool empty() const noexcept {
		return size_ == 0;
	}

	/**
	 * @brief Find the maximum possible key.
	 *
	 * @return key_value maximum possible key.
	 */
	inline key_value upper_bound() const noexcept {
		return index_.upper_bound();
	}

	/**
	 * @brief Find the minimum possible key.
	 * 
	 * @return key_value mimimum possible key.
	 */
	inline key_value lower_bound() const noexcept {
		return index_.lower_bound();
	}

	/**
	 * @brief Get the bit length of the keys.
	 * 
	 * @return bit length of the keys.
	 */
	inline size_t bit_length() const noexcept {
		return index_.bit_length();
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	bool contains(key_value key) const noexcept {
		if (empty()) return false;
		// Compute the partition that the key would belong to. If the partition exists and contains
		// the key then the trie contains that key. Otherwise, the trie does not contain the key.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		return partition != nullptr && partition->contains(key);
	}

	/**
	 * @brief Find the predecessor of a key.
	 * 
	 * @param key to find the predecessor of.
	 * @return some_key_value predecessor key if the predecessor exists. 
	 * @return none_key_value if the predecessor does not exist.
	 */
	some_key_value predecessor(key_value key) const noexcept {
		if (empty()) return some_key_value();

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;
		auto rep_key = node->key();

		// If the partition does not exist, then any key less than the given key cannot be in the
		// trie. In other words, there are no possible predecessors.
		if (partition == nullptr) return some_key_value();
		
		// TODO: == could possibly work instead of >=
		// If the current partition does not contain the predecessor, then the predecessor must be 
		// in the partition to the left. This happens when the key is the smallest value in the 
		// partition because the predecessor cannot be in the partition. We also know that 
		// the predecessor exists because we would have already returned if it didn't.
		if (partition->min().value() >= key) {
			// Compute the left representative node.
			auto left_node = node->get_left();

			// TODO: check if this is actually needed.
			// Make sure the left partition actually exists.
			if (left_node == nullptr) return some_key_value();

			// TODO: check if this is actually needed.
			// Set the partition to the left partition if it exists.
			auto left_key = left_node->key();
			if (partitions_.contains(left_key))
				partition = partitions_.at(left_key);
			else
				return some_key_value();
		}

		// Compute the predecessor.
		return partition->predecessor(key);
	}

	/**
	 * @brief Find the successor of a key.
	 * 
	 * @param key to find the successor of.
	 * @return some_key_value successor key if the successor exists.
	 * @return none_key_value if the successor does not exist.
	 */
	some_key_value successor(key_value key) const noexcept { 
		if (empty()) return some_key_value();

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;
		auto rep_key = node->key();

		// TODO: == could possibly work instead of <=
		// If the current partition does not contain the successor, then the successor must be 
		// in the partition to the right. This happens when the key is the smallest value in the 
		// partition because the successor cannot be in the partition. We also know that 
		// the successor exists because we would have already returned if it didn't.
		if (partition->max().value() <= key) {
			// Compute the right representative node.
			auto right_node = node->get_right();

			// TODO: check if this is actually needed.
			// Make sure the right partition actually exists.
			if (right_node == nullptr) return some_key_value();

			// TODO: check if this is actually needed.
			// Set the partition to the right partition if it exists.
			auto right_key = right_node->key();
			if (partitions_.contains(right_key))
				partition = partitions_.at(right_key);
			else
				return some_key_value();
		}

		// Compute the successor.
		return partition->successor(key);
	}

	/**
	 * @brief Find the minimum key.
	 * 
	 * @return some_key_value minimum key if trie is not empty.
	 * @return none_key_value if the trie is empty.
	 */
	some_key_value min() const noexcept { 
		if (empty()) return some_key_value(); 
		auto min_rep_key = index_.min().value();
		return partitions_.at(min_rep_key)->min();
	}

	/**
	 * @brief Find the maximum key.
	 * 
	 * @return some_key_value maximum key if trie is not empty.
	 * @return none_key_value if the trie is empty.
	 */
	some_key_value max() const noexcept { 
		if (empty()) return some_key_value(); 
		auto max_rep_key = index_.max().value();
		return partitions_.at(max_rep_key)->max(); 
	}

	/**
	 * @brief Insert a key into the trie.
	 * 
	 * @param key to insert into the trie.
	 */
	void insert(key_value key) {

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = create_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;
		auto rep_key = node->key();

		// Prevent double inserts.
		if (partition->contains(key)) return;

		// Insert the key into the partition.
		partition->insert(key);

		// If the partition has exceeded the maximum size, then we must split the partition or we
		// will be unable to meet the correct time complexity bounds.
		if (partition->size() > max_partition_size_) {

			// Remove the old partition.
			partitions_.remove(rep_key);
			index_.remove(rep_key);

			// TODO: Attempt to create 0 size after split condition in test cases.
			// Split the partition into two new partitions about the representative of the  median 
			// element. The maximum partition size is choosen such that the new partitions will 
			// always be nonempty.
			auto median = partition->median();
			auto pivot = get_representative(median);
			auto new_partitions = partition->split(pivot);

			// Insert the new partitions.
			for (auto new_partition : new_partitions) {
				assert(new_partition->size() > 0);
				// Insert the new partition.
				auto new_partition_max = new_partition->max().value();
				auto new_rep_key = get_representative(new_partition_max);
				index_.insert(new_rep_key);
				partitions_[new_rep_key] = new_partition;
			}
    	}

    	size_ += 1;
	}

	/**
	 * @brief Remove a key from the trie.
	 * 
	 * @param key to remove from the trie.
	 */
	void remove(key_value key) { 
		// Prevent double removes.
		if (empty()) return;

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;

		// Prevent double removes.
		if (partition == nullptr || !partition->contains(key)) return;

		// Get the representative key.
		auto rep_key = node->key();

		// Remove the key from the partition.
		partition->remove(key);

		// TODO: Add remove everything test case.
		// TODO: Test edge cases when splitting on a representative for insert and remove.
		
		// If the partition is empty, delete it.
		if (partition->empty()) {
			partitions_.remove(rep_key);
			index_.remove(rep_key);
		} 
		
		// Otherwise, if the partition is below the minimum size and we can merge it, do so.
		else if (partition->size() < min_partition_size_ && partitions_.size() > 1) {

			// Compute the two representatives of the partitions to merge. The particular choice 
			// does not matter, as long as we get two valid partitions to merge.
			node_ptr left_node = nullptr;
			node_ptr right_node = nullptr;
			if (node->get_left() != nullptr) {
				left_node = node->get_left();
				right_node = node;
			} else {
				left_node = node;
				right_node = node->get_right();
			}

			// Get the left and right partitions.
			auto left_rep_key = left_node->key();
			auto right_rep_key = right_node->key();
			auto left_partition = partitions_.at(left_rep_key);
			auto right_partition = partitions_.at(right_rep_key);
			
			// And remove them.
			partitions_.remove(left_rep_key);
			partitions_.remove(right_rep_key);
			index_.remove(left_rep_key);
			index_.remove(right_rep_key);

			// TODO: Improve this interface.
			// Merge the two partitions.
			auto merged_partition = left_partition->merge(left_partition, right_partition);

			// If the new merged partition exceeds the maximum size, we have to split it.
			if (merged_partition->size() > max_partition_size_) {
				
				// Split the partition into two new partitions about the representative of the  median 
				// element. The maximum partition size is choosen such that the new partitions will 
				// always be nonempty.
				auto median = merged_partition->median();
				auto pivot = get_representative(median);
				auto new_partitions = merged_partition->split(pivot);

				// Insert the new partitions.
				for (auto new_partition : new_partitions) {
					assert(new_partition->size() > 0);
					// Insert the new partition.
					auto new_partition_max = new_partition->max().value();
					auto new_rep_key = get_representative(new_partition_max);
					index_.insert(new_rep_key);
					partitions_[new_rep_key] = new_partition;
				}
			} 
			// Otherwise, simply insert the merged partition
			else {
				auto new_max_key = merged_partition->max().value();
				auto new_rep_key = get_representative(new_max_key);
				index_.insert(new_rep_key);
				partitions_[new_rep_key] = merged_partition;
			}
		}

		size_ -= 1;
	}
};

