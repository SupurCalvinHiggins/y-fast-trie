#pragma once
#include <assert.h>
#include <algorithm>
#include "x-fast-trie.h"

int main() {
	XFastTrie<uint8_t> trie;

	// test empty trie
	assert(trie.contains(0) == false);
	assert(trie.predecessor(0).has_value() == false);
	assert(trie.successor(0).has_value() == false);
	assert(trie.min().has_value() == false);
	assert(trie.max().has_value() == false);
	assert(trie.size() == 0);

	// test remove on empty trie
	trie.remove(0);
	assert(trie.contains(0) == false);
	assert(trie.predecessor(0).has_value() == false);
	assert(trie.successor(0).has_value() == false);
	assert(trie.min().has_value() == false);
	assert(trie.max().has_value() == false);
	assert(trie.size() == 0);

	// test contains and insert
	std::vector<uint8_t> values{12, 14, 13, 3, 17, 4, 9, 27, 30, 33, 1};
	for (auto x : values) {
		assert(trie.contains(x) == false);
		trie.insert(x);
		assert(trie.contains(x) == true);
	}

	// test predecessor and successor
	std::sort(values.begin(), values.end());
	for (size_t i = 1; i < values.size() - 1; ++i) {
		assert(trie.predecessor(values[i]).value() == values[i - 1]);
		assert(trie.successor(values[i]).value() == values[i + 1]);
	}

	// test size, min and max
	assert(trie.size() == values.size());
	assert(trie.min().value() == *std::min_element(values.begin(), values.end()));
	assert(trie.max().value() == *std::max_element(values.begin(), values.end()));
	return 0;

	// test remove
	for (auto x : values) {
		assert(trie.contains(x) == true);
		trie.remove(x);
		assert(trie.contains(x) == false);
	}

	// test size, min and max
	assert(trie.size() == 0);
	assert(trie.min().has_value() == false);
	assert(trie.max().has_value() == false);
}