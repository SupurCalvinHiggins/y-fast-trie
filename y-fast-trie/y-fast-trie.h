#pragma once 
#include <optional>
#include "../x-fast-trie/x-fast-trie.h"
#include "../x-fast-trie/x-fast-trie-map-wrapper.h"
#include "../red-black-tree/red-black-tree.h"

template <typename T>
class YFastTrie {

	using trie = XFastTrie<T>;
	using tree = RedBlackTree<T>;
	using tree_ptr = tree*;
	using rep_node_ptr = XFastTrieNode<T>*;

	using tree_ptrs = std::pair<tree_ptr, tree_ptr>;
	using tree_and_rep_node_ptrs = std::pair<tree_ptr, rep_node_ptr>;

	using key_value = T;
	using optional_key_value = std::optional<T>;

private:
	trie xfast;
	map_wrapper<key_value, tree_ptr> subtrees;
	size_t size_;
	key_value max_subtree_size;

	key_value get_representative(key_value key) {
		key_value mask = xfast.bits() - 1;
		// largest value that could exist in the partition that key would belong to
		return (key & ~mask) + mask;
	}

	tree_and_rep_node_ptrs get_subtree(key_value key, bool create_subtree = false) {
		// we need to find the rep_node for a given key
		// since the xfast searches for strict successors
		// we need to subtract one from the key value
		// in the 0 case, we do nothing because rep_node keys
		// are strictly greater than 0 for all valid tries
		auto pred_key = key == 0 ? 0 : key - 1;

		// find the rep_node for the given key
		auto rep_node = xfast.successor_node(pred_key);

		// if the rep_node is nullptr, the subtree does not exist
		// when we are inserting, we want to create the subtree
		// otherwise, we do not want to create nonexistant subtrees
		if (rep_node == nullptr && create_subtree) {
			// create the new representative and subtree
			auto rep = get_representative(key);
			xfast.insert(rep);
			rep_node = xfast.successor_node(rep - 1);
			subtrees[rep] = new tree();
		}

		return tree_and_rep_node_ptrs(rep_node, subtrees[rep_node->key()]);
	}
public:
    YFastTrie() {
		size_ = 0;
		max_subtree_size = xfast.bits() - 1;
	}
	~YFastTrie();
	bool contains(key_value key) {
		auto subtree_and_rep_node = get_subtree(key);
		auto subtree = subtree_and_rep_node.first;
		return subtree != nullptr && subtree->contains(key);
	}
	optional_key_value predecessor(key_value key) {

		// get the rep_node and subtree
		auto subtree_and_rep_node = get_subtree(key);
		auto subtree = subtree_and_rep_node.first;
		auto rep_node = subtree_and_rep_node.second;
		auto rep = rep_node->key();

		// if the subtree is nullptr, there cannot be a predecessor
		if (subtree == nullptr) return std::nullopt;
		
		// if the subtree does not contain the predecessor
		// it must be in the next subtree to the left
		if (subtree->min() >= key) {
			subtree = subtrees[rep_node->get_left()->key()];
		}

		return subtree->predecessor(key);
	}
	optional_key_value successor(key_value key) { 
		// get the rep_node and subtree
		auto subtree_and_rep_node = get_subtree(key);
		auto subtree = subtree_and_rep_node.first;
		auto rep_node = subtree_and_rep_node.second;
		auto rep = rep_node->key();

		// if the subtree is nullptr, there cannot be a successor
		if (subtree == nullptr) return std::nullopt;

		// if the subtree does not contain the successor
		// it must be in the next subtree to the right
		if (subtree->max() <= key) {
			subtree = subtrees[rep_node->get_right()->key()];
		}

		return subtree->successor(key);
	}
	optional_key_value min() { 
		if (size_ == 0) return std::nullopt; 
		return subtrees[xfast.min()]->min();
	}
	optional_key_value max() { 
		if (size_ == 0) return std::nullopt;
		return subtrees[xfast.max()]->max(); 
	}
	size_t size() { return size_; };
	const key_value limit() { return -1; }
	void insert(key_value key) {

		// get the rep_node and subtree
		// if they do not exist, create them
		auto subtree_and_rep_node = get_subtree(key, true);
		auto subtree = subtree_and_rep_node.first;
		auto rep_node = subtree_and_rep_node.second;
		auto rep = rep_node->key();

		// check to make sure that the key is not already stored
		if (subtree->contains(key)) return;

		// insert the key into the correct subtree
		subtree->insert(key);

		// if the subtree has exceeded the maximum size
		// we need to split the subtree into two new subtrees
		// otherwise, we will be unable to meet our time complexity bounds
		if (subtree->size() > max_subtree_size) {

			// delete the current subtree and representative
			subtrees[rep] = nullptr;
			xfast.remove(rep);

			// split the subtree into two new subtrees
			auto new_subtrees = subtree->split();

			// insert the new subtrees and reps
			for (auto new_subtree : new_subtrees) {
				auto new_rep = get_representative(new_subtree->max());
				xfast.insert(new_rep);
				subtrees[new_rep] = new_subtree;
			}
    	}

    	size_ += 1;
	}
	void remove(key_value key) { 
		// get the rep_node and subtree
		auto subtree_and_rep_node = get_subtree(key);
		auto subtree = subtree_and_rep_node.first;
		auto rep_node = subtree_and_rep_node.second;
		auto rep = rep_node->key();

		// break if the trie does not contain the key
		if (subtree == nullptr || !subtree->contains(key)) return;

		subtree->remove(key);

		if (subtree->size() == 0) {

		} else if (subtree->size() < min_subtree_size && subtrees.size() > 1) {
			rep_node_ptr left_rep = nullptr;
			rep_node_ptr right_rep = nullptr;
			if (rep_node->get_left() != nullptr) {
				left_rep = rep_node->get_left();
				right_rep = rep_node;
			} else {
				right_rep = rep_node;
				left_rep = rep_node->get_right();
			}
		}

		return; 
	}
};

