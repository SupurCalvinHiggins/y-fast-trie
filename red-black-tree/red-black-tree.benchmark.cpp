#include <benchmark/benchmark.h>
#include <cstdint>
#include "red-black-tree.h"

template <typename T>
class RedBlackTreeFixture: public benchmark::Fixture {
protected:
	void SetUp(const benchmark::State& state) {
		std::srand(0);
		for (int i = 0; i < state.range(0); ++i) {
			auto val = std::rand() % tree.limit();
			tree.insert(val);
		}
	}

	virtual void BM_RedBlackTree_insert(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree);
			state.PauseTiming();
			auto contained = tree.contains(val);	
			benchmark::ClobberMemory();
			tree.remove(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
			tree.insert(val);
			benchmark::ClobberMemory();
			state.PauseTiming();
			if (!contained) tree.remove(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
		}
	}

	virtual void BM_RedBlackTree_remove(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree);
			state.PauseTiming();
			auto contained = tree.contains(val);
			benchmark::ClobberMemory();
			tree.insert(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
			tree.remove(val);
			benchmark::ClobberMemory();
			state.PauseTiming();
			if (contained) tree.insert(val);
			benchmark::ClobberMemory();
			state.ResumeTiming();
		}
	}

	virtual void BM_RedBlackTree_predecessor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.predecessor(val));
		}
	}

	virtual void BM_RedBlackTree_successor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.successor(val));
		}
	}

	virtual void BM_RedBlackTree_contains(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.contains(val));
		}
	}

	RedBlackTree<T> tree;
};

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint64_t, uint64_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint32_t, uint32_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint16_t, uint16_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint8_t, uint8_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_insert(state);
}

BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_insert_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint64_t, uint64_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint32_t, uint32_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint16_t, uint16_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint8_t, uint8_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_remove(state);
}

BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_remove_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint64_t, uint64_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint32_t, uint32_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint16_t, uint16_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint8_t, uint8_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_predecessor(state);
}

BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_predecessor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint64_t, uint64_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint32_t, uint32_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint16_t, uint16_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint8_t, uint8_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_successor(state);
}

BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_successor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint64_t, uint64_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint32_t, uint32_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint16_t, uint16_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint8_t, uint8_t) (benchmark::State& state) {
	RedBlackTreeFixture::BM_RedBlackTree_contains(state);
}

BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(RedBlackTreeFixture, BM_RedBlackTree_contains_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_MAIN();