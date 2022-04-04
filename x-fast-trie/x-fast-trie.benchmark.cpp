#include <benchmark/benchmark.h>
#include <cstdint>
#include "x-fast-trie.h"

template <typename T>
class XFastTrieFixture: public benchmark::Fixture {
protected:
	void SetUp(const benchmark::State& state) {
		std::srand(0);
		for (int i = 0; i < state.range(0); ++i) {
			auto val = std::rand() % trie.upper_bound();
			trie.insert(val);
		}
	}

	virtual void BM_XFastTrie_insert(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % trie.upper_bound();
			benchmark::DoNotOptimize(trie);
			state.PauseTiming();
			auto contained = trie.contains(val);	
			benchmark::ClobberMemory();
			trie.remove(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
			trie.insert(val);
			benchmark::ClobberMemory();
			state.PauseTiming();
			if (!contained) trie.remove(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
		}
	}

	virtual void BM_XFastTrie_remove(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % trie.upper_bound();
			benchmark::DoNotOptimize(trie);
			state.PauseTiming();
			auto contained = trie.contains(val);
			benchmark::ClobberMemory();
			trie.insert(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
			trie.remove(val);
			benchmark::ClobberMemory();
			state.PauseTiming();
			if (contained) trie.insert(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
		}
	}

	virtual void BM_XFastTrie_predecessor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % trie.upper_bound();
			benchmark::DoNotOptimize(trie.predecessor(val));
		}
	}

	virtual void BM_XFastTrie_successor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % trie.upper_bound();
			benchmark::DoNotOptimize(trie.successor(val));
		}
	}

	virtual void BM_XFastTrie_contains(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % trie.upper_bound();
			benchmark::DoNotOptimize(trie.contains(val));
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

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_remove_uint64_t, uint64_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_remove_uint32_t, uint32_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_remove_uint16_t, uint16_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_remove_uint8_t, uint8_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_remove(state);
}

BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_remove_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_remove_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_remove_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_remove_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint64_t, uint64_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint32_t, uint32_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint16_t, uint16_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint8_t, uint8_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_predecessor(state);
}

BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_predecessor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_successor_uint64_t, uint64_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_successor_uint32_t, uint32_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_successor_uint16_t, uint16_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_successor_uint8_t, uint8_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_successor(state);
}

BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_successor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_successor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_successor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_successor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_contains_uint64_t, uint64_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_contains_uint32_t, uint32_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_contains_uint16_t, uint16_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(XFastTrieFixture, BM_XFastTrie_contains_uint8_t, uint8_t) (benchmark::State& state) {
	XFastTrieFixture::BM_XFastTrie_contains(state);
}

BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_contains_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_contains_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_contains_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(XFastTrieFixture, BM_XFastTrie_contains_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_MAIN();