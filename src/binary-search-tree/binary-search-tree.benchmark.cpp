#include "binary-search-tree.h"
#include <cstdint>
#include <benchmark/benchmark.h>

template <typename T>
class BinarySearchTreeFixture: public benchmark::Fixture {
protected:
	void SetUp(const benchmark::State& state) {
		std::srand(0);
		for (int i = 0; i < state.range(0); ++i) {
			auto val = std::rand() % tree.limit();
			tree.insert(val);
		}
	}

	virtual void BM_BinarySearchTree_insert(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree);
			auto contained = tree.contains(val);	
			benchmark::ClobberMemory();
			tree.remove(val);
			benchmark::ClobberMemory();
            auto start = std::chrono::high_resolution_clock::now();
            benchmark::ClobberMemory();
			tree.insert(val);
            benchmark::ClobberMemory();
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            state.SetIterationTime(elapsed_seconds.count());
			if (!contained) tree.remove(val);
			benchmark::ClobberMemory();
		}
	}

	virtual void BM_BinarySearchTree_remove(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree);
			auto contained = tree.contains(val);
			benchmark::ClobberMemory();
			tree.insert(val);
			benchmark::ClobberMemory();
            auto start = std::chrono::high_resolution_clock::now();
            benchmark::ClobberMemory();
			tree.remove(val);
        	benchmark::ClobberMemory();
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            state.SetIterationTime(elapsed_seconds.count());
			if (contained) tree.insert(val);
			benchmark::ClobberMemory();
		}
	}

	virtual void BM_BinarySearchTree_predecessor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.predecessor(val));
		}
	}

	virtual void BM_BinarySearchTree_successor(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.successor(val));
		}
	}

	virtual void BM_BinarySearchTree_contains(benchmark::State& state) {
		for (auto _ : state) {
			auto val = std::rand() % tree.limit();
			benchmark::DoNotOptimize(tree.contains(val));
		}
	}

	BinarySearchTree<T> tree;
};

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint64_t, uint64_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint32_t, uint32_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint16_t, uint16_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_insert(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint8_t, uint8_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_insert(state);
}

BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_insert_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint64_t, uint64_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint32_t, uint32_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint16_t, uint16_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_remove(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint8_t, uint8_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_remove(state);
}

BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_remove_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint64_t, uint64_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint32_t, uint32_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint16_t, uint16_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_predecessor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint8_t, uint8_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_predecessor(state);
}

BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_predecessor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint64_t, uint64_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint32_t, uint32_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint16_t, uint16_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_successor(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint8_t, uint8_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_successor(state);
}

BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_successor_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint64_t, uint64_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint32_t, uint32_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint16_t, uint16_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_contains(state);
}

BENCHMARK_TEMPLATE_DEFINE_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint8_t, uint8_t) (benchmark::State& state) {
	BinarySearchTreeFixture::BM_BinarySearchTree_contains(state);
}

BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint64_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint32_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint16_t)->RangeMultiplier(16)->Range(1, 1<<16);
BENCHMARK_REGISTER_F(BinarySearchTreeFixture, BM_BinarySearchTree_contains_uint8_t)->RangeMultiplier(16)->Range(1, 1<<16);

BENCHMARK_MAIN();