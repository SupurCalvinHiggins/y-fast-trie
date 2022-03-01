#pragma once
#include <unordered_map>

template <typename K, typename V>
class map_wrapper {
private:
	std::unordered_map<K, V> map_;
public:
	bool contains(K key) { return map_.count(key) != 0; }
	void insert(K key, V val) { map_.insert({key, val}); }
	void remove(K key) { map_.erase(map_.find(key)); };
	size_t size() { return map_.size(); }
	bool empty() { return map_.empty(); }
	V at(K key) { return map_.at(key); }
	std::unordered_map<K, V> data() { return map_; }
};