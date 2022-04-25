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
	using partition_ptrs = std::array<partition_ptr, 2>;
	using partition_index_type = map_wrapper<key_type, partition_ptr>;

private:
	index_type index_;
	partition_index_type partitions_; 
	size_type size_;
	bool animate_;

private:
	// Mask for computing representatives.
	static constexpr key_type partition_mask_ = index_type::bit_length() - 1;
	
	// Maximum partition size.
	static constexpr key_type max_partition_size_ = index_type::bit_length() * 2;
	
	// Minimum partition size.
	static constexpr key_type min_partition_size_ = index_type::bit_length() / 2;

private:

	/**
	 * @brief Get the representative node for a given key.
	 * 
	 * @param key to get the representative node for.
	 * @return representative node.
	 */
	inline node_ptr get_representative_node(key_type key)  noexcept {
		// We compute the inclusive successor node of the key. This is the representative node of 
		// the partition that the key should be in.
		return index_.get_inclusive_successor_node(key);
	}

	/**
	 * @brief Get the partition from a representative node.
	 * 
	 * @param representative node of the partition.
	 * @return the partition of the node.
	 */
	inline partition_ptr get_partition(node_ptr node)  noexcept {
		// We check to make sure the representative node exists and then we find the partition.
		if (node == nullptr) return nullptr;
		return partitions_.at(node->key());
	}

	/**
	 * @brief Get the partition from a representative node.
	 * 
	 * @param representative node of the partition.
	 * @return the partition of the node.
	 */
	inline partition_ptr get_partition(key_type key)  noexcept {
		auto node = get_representative_node(key);
		return get_partition(node);
	}

	/**
	 * @brief Insert a partition into the trie.
	 * 
	 * @param rep_key of the partition.
	 * @param partition to insert.
	 */
	inline void insert_partition(key_type rep_key, partition_ptr partition) noexcept {
		index_.insert(rep_key);
		partitions_[rep_key] = partition;
	}

	/**
	 * @brief Insert a partition into the trie.
	 * 
	 * @param partition to insert.
	 */
	inline void insert_partition(partition_ptr partition) noexcept {
		auto rep_key = partition->max().value();
		insert_partition(rep_key, partition);
	}

	/**
	 * @brief Remove a partition from the trie.
	 * 
	 * @param rep_key of the partition to remove.
	 */
	inline void remove_partition(key_type rep_key) noexcept {
		index_.remove(rep_key);
		partitions_.remove(rep_key);
	}

	/**
	 * @brief Remove a partition from the trie.
	 * 
	 * @param representative node of the partition to remove.
	 */
	inline void remove_partition(node_ptr node) noexcept {
		remove_partition(node->key());
	}

	/**
	 * @brief Create the default partition.
	 * 
	 * @return the default partition.
	 */
	inline partition_ptr create_default_partition() noexcept {
		auto partition = new partition_type();
		insert_partition(upper_bound(), partition);
		return partition;
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
    YFastTrie() : size_(0), animate_(true) {  }

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
		return size_ == 0;
	}

	/**
	 * @brief Check if the trie contains a key.
	 * 
	 * @param key to check if the trie contains.
	 * @return true if the trie contains the key.
	 * @return false if the trie does not contain the key.
	 */
	bool contains(key_type key)  noexcept(NEX) {
		if (empty()) return false;
		// Compute the partition that the key would belong to. If the partition exists and contains
		// the key then the trie contains that key. Otherwise, the trie does not contain the key.
		auto node = get_representative_node(key);
		auto partition = get_partition(node);
		auto res = partition != nullptr && partition->contains(key);
		CLEAN();
		return res;
	}

	/**
	 * @brief Find the predecessor of a key.
	 * 
	 * @param key to find the predecessor of.
	 * @return some_key_type predecessor key if the predecessor exists. 
	 * @return none_key_type if the predecessor does not exist.
	 */
	some_key_type predecessor(key_type key)  noexcept(NEX) {
		if (empty()) return some_key_type();

		// Compute the partition and representative node that the key would belong to.
		auto node = get_representative_node(key);
		auto partition = get_partition(node);

		// If the partition does not exist, then any key less than the given key cannot be in the
		// trie. In other words, there are no possible predecessors. This also handles the case
		// where the representative node does not exist because the partition does not exist, if
		// and only if the representative node does not exist.
		if (partition == nullptr) return some_key_type();

		// If the current partition does not contain the predecessor, then the predecessor must be 
		// in the partition to the left. This happens when the key is the smallest value in the 
		// partition because the predecessor cannot be in the partition. We also know that 
		// the predecessor exists because we would have already returned if it didn't.
		if (partition->min().value() >= key) {
			// Compute the left representative node.
			auto left_node = node->get_left();
			index_.MARK_AND_UPDATE(left_node);

			// Make sure the left partition actually exists.
			if (left_node == nullptr) return some_key_type();

			// Set the partition to the left partition.
			partition = get_partition(left_node);
		}

		// Compute the predecessor.
		auto res = partition->predecessor(key);
		CLEAN();
		return res;
	}

	/**
	 * @brief Find the successor of a key.
	 * 
	 * @param key to find the successor of.
	 * @return some_key_type successor key if the successor exists.
	 * @return none_key_type if the successor does not exist.
	 */
	some_key_type successor(key_type key)  noexcept(NEX) { 
		if (empty()) return some_key_type();

		// Compute the partition and representative node that the key would belong to.
		auto node = get_representative_node(key);
		auto partition = get_partition(node);

		// If the partition does not exist, then any key greater than the given key cannot be in the
		// trie. In other words, there are no possible successors. This also handles the case
		// where the representative node does not exist because the partition does not exist, if
		// and only if the representative node does not exist.
		if (partition == nullptr) return some_key_type();

		// If the current partition does not contain the successor, then the successor must be 
		// in the partition to the right. This happens when the key is the smallest value in the 
		// partition because the successor cannot be in the partition. We also know that 
		// the successor exists because we would have already returned if it didn't.
		if (partition->max().value() <= key) {
			// Compute the right representative node.
			auto right_node = node->get_right();
			index_.MARK_AND_UPDATE(right_node);


			// Make sure the right partition actually exists.
			if (right_node == nullptr) return some_key_type();

			// Set the partition to the right partition.
			partition = get_partition(right_node);
		}

		// Compute the successor.
		auto res =  partition->successor(key);
		CLEAN();
		return res;
	}

	/**
	 * @brief Find the minimum key.
	 * 
	 * @return some_key_type minimum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type min()  noexcept(NEX) { 
		if (empty()) return some_key_type(); 
		auto min_rep_key = index_.min().value();
		return get_partition(min_rep_key)->min();
	}

	/**
	 * @brief Find the maximum key.
	 * 
	 * @return some_key_type maximum key if trie is not empty.
	 * @return none_key_type if the trie is empty.
	 */
	some_key_type max()  noexcept(NEX) { 
		if (empty()) return some_key_type(); 
		auto max_rep_key = index_.max().value();
		return get_partition(max_rep_key)->max(); 
	}

	/**
	 * @brief Insert a key into the trie.
	 * 
	 * @param key to insert into the trie.
	 */
	void insert(key_type key) noexcept(NEX) {
		// Compute the partition and representative node that the key would belong to.
		auto node = get_representative_node(key);
		auto partition = !node ? create_default_partition() : get_partition(node);

		// Prevent double inserts.
		if (partition->contains(key))  {
			CLEAN();
			return;
		}

		// Insert the key into the partition.
		partition->insert(key);

		// If the partition has exceeded the maximum size, then we must split the partition or we
		// will be unable to meet the correct time complexity bounds.
		if (partition->size() > max_partition_size_) {
			// First, we remove the original partition from the trie.
			remove_partition(node);
			// Then, we split the original partition and insert the new partitions.
			auto new_partitions = partition->split();
			for (auto new_partition : new_partitions)
				insert_partition(new_partition);
    	}

    	size_ += 1;
		CLEAN();
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
		auto node = get_representative_node(key);
		auto partition = get_partition(node);

		// Prevent double removes.
		if (partition == nullptr || !partition->contains(key)) {
			CLEAN();
			return;
		} 

		// Remove the key from the partition.
		partition->remove(key);
		
		// If the partition is empty, delete it.
		if (partition->empty()) 
			remove_partition(node);
		
		// Otherwise, if the partition is below the minimum size and we can merge it, do so.
		else if (partition->size() < min_partition_size_ && partitions_.size() > 1) {

			// Compute the two representatives of the partitions to merge. The particular choice 
			// does not matter, as long as we get two valid partitions to merge.
			auto left_node  = node;
			auto right_node = node->get_right() ? node->get_right() : node->get_left();
			
			// Ensure that the left partition has keys that are < the right partition.
			if (left_node->key() > right_node->key())
				std::swap(left_node, right_node);
			
			// Remove the original partitions from the trie.
			auto left_partition = get_partition(left_node);
			auto right_partition = get_partition(right_node);
			remove_partition(left_node);
			remove_partition(right_node);

			// Merge the two original partitions.
			auto merged_partition = left_partition->merge(left_partition, right_partition);

			// If the new merged partition exceeds the maximum size, we have to split it.
			if (merged_partition->size() > max_partition_size_) {
				// Split the partition and insert the new partitions.
				auto new_partitions = partition->split();
				for (auto new_partition : new_partitions)
					insert_partition(new_partition);
			} 
			// Otherwise, simply insert the merged partition
			else {
				insert_partition(merged_partition);
			}
		}

		size_ -= 1;
		CLEAN();
	}

	virtual ~YFastTrie() = default;

private:
	// Extra private methods for the demo program.

	/**
	 * @brief Converts a pointer into a DOT string.
	 * 
	 * @param ptr to convert. 
	 * @return DOT string of the pointer.
	 */
    std::string ptr_to_str(void* ptr) noexcept {
        std::ostringstream ss;
        ss << ptr;
        return "n" + ss.str();
    }

    void CLEAN() {
        index_.CLEAN_NO_UPDATE();
        for (auto& pair : partitions_) 
            pair.second->CLEAN_NO_UPDATE();
		UPDATE_GUI();
    }

public:
	// Extra public methods for the demo program.

	void set_animate(bool animate) {
		index_.set_animate(animate);
		for (auto pair : partitions_)
			pair.second->set_animate(animate);
	}

	/**
	 * @brief Build a string in the DOT format representing the trie.
	 * 
	 * @return DOT string of the trie.
	 */
	std::string to_dot() noexcept {
		std::string output;
		output += index_.to_dot();
		output.pop_back();
		for ( auto& pair : partitions_) {
			auto partition_output = pair.second->to_dot();
			partition_output.erase(0, 7);
			auto found = partition_output.find("n");
            try {
                auto root_str  = partition_output.substr(found, 15);
                auto index_str = ptr_to_str(index_.lss_.back().at(pair.first));
                output += index_str + " -> " + root_str + ";\n";
                output += "subgraph" + partition_output + "\n";
            } catch(...) { 
                // FUCK NULL RB_TREES SFJKLDSJKL:LGKSJDKL:GNHKEWkhl;GHEWGHKL:;hgsr
                // four fucking hours
                // std::cout << partition_output << std::endl;
                // assert(false); 
            }
			
		}
		return output + "}";
	}
};

