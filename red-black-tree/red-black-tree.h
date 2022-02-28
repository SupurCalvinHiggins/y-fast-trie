#pragma once


template <typename T>
class RedBlackTree {
public:
	void insert(T key);
	void remove(T key);
	bool contains(T key);
	size_t size();
};