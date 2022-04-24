/**
 * @file x-fast-trie-map-wrapper.h
 * @author Calvin Higgins (calvin_higgins2@uri.edu)
 * @brief Wrapper class for std::unordered_map.
 * @version 1.0
 * @date 2022-04-24
 * 
 */

#pragma once
#include <unordered_map>

/**
 * @brief Wrapper class for std::unordered_map.
 * 
 * @tparam Key_ is the key type.
 * @tparam Value_ is the value type.
 */
template <typename Key_, typename Value_>
class map_wrapper : public std::unordered_map<Key_, Value_> {
public:
	using key_type = Key_;
	using value_type = Value_;

public:
	/**
	 * @brief Check if a key is in the hash table.
	 * 
	 * @param key to check.
	 * @return true if the key is in the hash table.
	 * @return false if the key is not in the hash table.
	 */
	bool contains(key_type key) const { 
		return this->count(key) != 0; 
	}

	/**
	 * @brief Remove a key from the hash table.
	 * 
	 * @param key to remove from the hash table.
	 */
	void remove(key_type key) { 
		this->erase(this->find(key)); 
	}
};