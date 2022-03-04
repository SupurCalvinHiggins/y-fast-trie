#include <benchmark/benchmark.h>
#include <cstdint>
#include "x-fast-trie.h"

static void seed(const benchmark::State& state) {
	std::srand(0);
}

static void BM_XFastTrie_n_insert(benchmark::State& state) {
	for (auto _ : state) {
		XFastTrie<uint64_t> trie;
		for (int i = 0; i < state.range(0); ++i)  {
			auto val = std::rand() % trie.limit();
			trie.insert(val);
		}
	}
}
BENCHMARK(BM_XFastTrie_n_insert)->RangeMultiplier(2)->Range(1, 1<<16)->Setup(seed);

static void BM_XFastTrie_1_predecessor_on_n_trie(benchmark::State& state) {
	XFastTrie<uint64_t> trie;
	for (int i = 0; i < state.range(0); ++i) {
		auto val = std::rand() % trie.limit();
		trie.insert(val);
	}

	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.predecessor(val));
	}
}
BENCHMARK(BM_XFastTrie_1_predecessor_on_n_trie)->RangeMultiplier(2)->Range(1, 1<<16)->Setup(seed);

static void BM_XFastTrie_1_successor_on_n_trie(benchmark::State& state) {
	XFastTrie<uint64_t> trie;
	for (int i = 0; i < state.range(0); ++i) {
		auto val = std::rand() % trie.limit();
		trie.insert(val);
	}

	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.successor(val));
	}
}
BENCHMARK(BM_XFastTrie_1_successor_on_n_trie)->RangeMultiplier(2)->Range(1, 1<<16)->Setup(seed);

static void BM_XFastTrie_1_contains_on_n_trie(benchmark::State& state) {
	XFastTrie<uint64_t> trie;
	for (int i = 0; i < state.range(0); ++i) {
		auto val = std::rand() % trie.limit();
		trie.insert(val);
	}

	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.contains(val));
	}
}
BENCHMARK(BM_XFastTrie_1_contains_on_n_trie)->RangeMultiplier(2)->Range(1, 1<<16)->Setup(seed);

BENCHMARK_MAIN();