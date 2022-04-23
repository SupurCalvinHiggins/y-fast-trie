#pragma once 
#include "../x-fast-trie/x-fast-trie.h"
#include "../x-fast-trie/x-fast-trie-map-wrapper.h"
#include "../red-black-tree/red-black-tree.h"
#include "../constants.h"
#include <optional>
#include <vector>
#include <assert.h>
#include <type_traits>
#include <string>

/**
 * @brief Data structure for fast dynamic ordered set operations on a bounded universe.
 * 
 * @tparam Key_ is the key type.
 * @tparam Bucket_ is the bucket type.
 */
template <typename Key_, typename Bucket_ = RedBlackTree<Key_>>
class YFastTrie {
public:
	using key_type = Key_;
	static_assert(std::is_unsigned<key_type>::value, "Key type must be an unsigned integer.");

	using some_key_type = std::optional<key_type>;
	using size_type = size_t;

private:
	using index_type = XFastTrie<key_type>;
	using node_ptr = typename index_type::node_ptr;

	using partition_type = Bucket_;
	using partition_ptr = partition_type*;
	using partition_and_node_ptr_pair = std::pair<partition_ptr, node_ptr>;

	using partition_index_type = map_wrapper<key_type, partition_ptr>;

private:
	index_type index_;
	partition_index_type partitions_; 
	size_type size_;

private:
	// Mask for computing representatives.
	static constexpr key_type partition_mask_ = index_type::bit_length() - 1;
	
	// Maximum partition size.
	static constexpr key_type max_partition_size_ = index_type::bit_length() * 2;
	
	// Minimum partition size.
	static constexpr key_type min_partition_size_ = index_type::bit_length() / 2;

private:

	/**
	 * @brief Get the representative key from a given key.
	 * 
	 * @param key to get the representative of.
	 * @return representative of the key.
	 */
	inline key_type get_representative(key_type key) const noexcept {
		return (key & ~partition_mask_) + partition_mask_;
	}

	/**
	 * @brief Get the partition and representative node of a key.
	 * 
	 * @param key to get the partition and representative node of.
	 * @return the partition and representative node.
	 */
	partition_and_node_ptr_pair get_partition_and_node(key_type key) const noexcept(NEX) {
		// Since the index searches for strict successors, we need to subtract 1. However, we do not
		// need to subtract 1 from 0 since the representative keys are strictly greater than zero.
		auto pred_key = key == 0 ? 0 : key - 1;
		auto node = index_.get_successor_node(pred_key);

		// Now that we have computed the representative node, we can compute the partition that
		// the representative node belongs to. If the node does not exist, then the partition also
		// does not exist.
		if (node == nullptr)
			return partition_and_node_ptr_pair(nullptr, nullptr);
		
		// Compute the partition.
		auto partition = partitions_.at(node->key());

		return partition_and_node_ptr_pair(partition, node);
	}

	/**
	 * @brief Create the partition and representative node of a key if they do not exist.
	 * 
	 * @param key to create the partition and representative node of.
	 * @return the partition and representative node.
	 */
	partition_and_node_ptr_pair create_partition_and_node(key_type key) noexcept(NEX) {
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
			partitions_[rep_key] = new partition_type();
		}

		// Compute the partition.
		auto partition = partitions_.at(node->key());

		return partition_and_node_ptr_pair(partition, node);
	}

public:
	/**
	 * @brief Find the maximum possible key.
	 *
	 * @return key_type maximum possible key.
	 */
	static constexpr key_type upper_bound() noexcept {
		return index_type::upper_bound();
	}

	/**
	 * @brief Find the minimum possible key.
	 * 
	 * @return key_type mimimum possible key.
	 */
	static constexpr key_type lower_bound() noexcept {
		return index_type::lower_bound();
	}

	/**
	 * @brief Get the bit length of the keys.
	 * 
	 * @return bit length of the keys.
	 */
	static constexpr size_type bit_length() noexcept {
		return index_type::bit_length();
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
		return size_ == 0;
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	bool contains(key_type key) const noexcept(NEX) {
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
	 * @return some_key_type predecessor key if the predecessor exists. 
	 * @return none_key_type if the predecessor does not exist.
	 */
	some_key_type predecessor(key_type key) const noexcept(NEX) {
		if (empty()) return some_key_type();

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;

		// If the partition does not exist, then any key less than the given key cannot be in the
		// trie. In other words, there are no possible predecessors. This also handles the case
		// where the representative node does not exist because the partition does not exist, if
		// and only if the representative node does not exist.
		if (partition == nullptr) return some_key_type();

		auto rep_key = node->key();
		
		// If the current partition does not contain the predecessor, then the predecessor must be 
		// in the partition to the left. This happens when the key is the smallest value in the 
		// partition because the predecessor cannot be in the partition. We also know that 
		// the predecessor exists because we would have already returned if it didn't.
		if (partition->min().value() >= key) {
			// Compute the left representative node.
			auto left_node = node->get_left();

			// Make sure the left partition actually exists.
			if (left_node == nullptr) return some_key_type();

			// Set the partition to the left partition.
			auto left_key = left_node->key();
			partition = partitions_.at(left_key);
		}

		// Compute the predecessor.
		return partition->predecessor(key);
	}

	/**
	 * @brief Find the successor of a key.
	 * 
	 * @param key to find the successor of.
	 * @return some_key_type successor key if the successor exists.
	 * @return none_key_type if the successor does not exist.
	 */
	some_key_type successor(key_type key) const noexcept(NEX) { 
		if (empty()) return some_key_type();

		// Compute the partition and representative node that the key would belong to.
		auto partition_and_node = get_partition_and_node(key);
		auto partition = partition_and_node.first;
		auto node = partition_and_node.second;

		// If the partition does not exist, then any key greater than the given key cannot be in the
		// trie. In other words, there are no possible successors. This also handles the case
		// where the representative node does not exist because the partition does not exist, if
		// and only if the representative node does not exist.
		if (partition == nullptr) return some_key_type();

		auto rep_key = node->key();

		// If the current partition does not contain the successor, then the successor must be 
		// in the partition to the right. This happens when the key is the smallest value in the 
		// partition because the successor cannot be in the partition. We also know that 
		// the successor exists because we would have already returned if it didn't.
		if (partition->max().value() <= key) {
			// Compute the right representative node.
			auto right_node = node->get_right();

			// Make sure the right partition actually exists.
			if (right_node == nullptr) return some_key_type();

			// Set the partition to the right partition.
			auto right_key = right_node->key();
			partition = partitions_.at(right_key);
		}

		// Compute the successor.
		return partition->successor(key);
	}

	/**
	 * @brief Find the minimum key.
	 * 
	 * @return some_key_type minimum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type min() const noexcept(NEX) { 
		if (empty()) return some_key_type(); 
		auto min_rep_key = index_.min().value();
		return partitions_.at(min_rep_key)->min();
	}

	/**
	 * @brief Find the maximum key.
	 * 
	 * @return some_key_type maximum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type max() const noexcept(NEX) { 
		if (empty()) return some_key_type(); 
		auto max_rep_key = index_.max().value();
		return partitions_.at(max_rep_key)->max(); 
	}

	/**
	 * @brief Insert a key into the trie.
	 * 
	 * @param key to insert into the trie.
	 */
	void insert(key_type key) noexcept(NEX) {

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
	void remove(key_type key) noexcept(NEX) { 
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

	virtual ~YFastTrie() = default;
};

