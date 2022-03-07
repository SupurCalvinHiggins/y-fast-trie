#include <benchmark/benchmark.h>
#include <cstdint>
#include <iostream>
#include "x-fast-trie.h"

template <typename T>
class XFastTrieFixture: public benchmark::Fixture {
protected:
	void SetUp(const benchmark::State& state) {
		// std::cout << "state.range(0)=" << state.range(0) << std::endl; 
		std::srand(0);
		for (int i = 0; i < state.range(0); ++i) {
			auto val = std::rand() % trie.limit();
			// std::cout << i << ' ' << val << ' ' << std::endl;
			trie.insert(val);
		}
	}

	virtual void BM_XFastTrie_insert(benchmark::State& state) {
		// std::cout << "begin bench" << std::endl;
		for (auto _ : state) {
			auto val = std::rand() % trie.limit();
			benchmark::DoNotOptimize(trie);
			// std::cout << "begin insert " << val << std::endl;
			trie.insert(val);
			// std::cout << "end insert" << std::endl;
			benchmark::ClobberMemory();
			state.PauseTiming();
			// std::cout << "begin remove " << val << std::endl;
			trie.remove(val);
			// std::cout << "end remove" << std::endl;
			benchmark::ClobberMemory();
			state.ResumeTiming();
		}
	}

	XFastTrie<T> trie;
};

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_insert_uint64_t, uint64_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_insert_uint32_t, uint32_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_insert_uint16_t, uint16_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_insert_uint8_t, uint8_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_insert(state);
}

BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_insert_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_insert_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_insert_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_insert_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_predecessor, uint64_t) (benchmark::State& state) {
	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.predecessor(val));
	}
}
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_predecessor)->RangeMultiplier(16)->Range(1, 1<<16);


BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_successor, uint64_t) (benchmark::State& state) {
	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.successor(val));
	}
}
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_successor)->RangeMultiplier(16)->Range(1, 1<<16);


BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_contains, uint64_t) (benchmark::State& state) {
	for (auto _ : state) {
		auto val = std::rand() % trie.limit();
		benchmark::DoNotOptimize(trie.contains(val));
	}
}
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_contains)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_MAIN();