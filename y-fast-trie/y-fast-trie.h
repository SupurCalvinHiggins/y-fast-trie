#pragma once 
#include <optional>

template <typename T>
class YFastTrie {
public:
    YFastTrie();
	~YFastTrie();
	bool contains(T key);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	std::optional<T> min();
	std::optional<T> max();
	size_t size() { return size_; };
	const T limit() { return -1; }
	void insert(T key);
	void remove(T key);
};

