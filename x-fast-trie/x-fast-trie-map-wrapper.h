#pragma once
#include <unordered_map>

template <typename K, typename V>
class map_wrapper : public std::unordered_map<K, V> {
public:
	bool contains(K key) { return this->count(key) != 0; }
	void remove(K key) { this->erase(this->find(key)); }
};