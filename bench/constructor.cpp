#include <string>
using namespace std::string_literals;

#include <benchmark/benchmark.h>

#include <Igor/StaticVector.hpp>

static void BM_StaticVectorCreation_Int(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  for (auto _ : state) {
    Igor::StaticVector<int, CAPACITY> vec(512, 42);
    benchmark::DoNotOptimize(vec.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorCopy_Int(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  Igor::StaticVector<int, CAPACITY> v1(512, 42);
  for (auto _ : state) {
    Igor::StaticVector<int, CAPACITY> v2(v1);
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorCopy_Int_DifferentSize(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  Igor::StaticVector<int, 2 * CAPACITY> v1(512, 42);
  for (auto _ : state) {
    Igor::StaticVector<int, CAPACITY> v2(v1);
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorMove_Int(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  for (auto _ : state) {
    state.PauseTiming();
    Igor::StaticVector<int, CAPACITY> v1(512, 42);
    state.ResumeTiming();
    Igor::StaticVector<int, CAPACITY> v2(std::move(v1));
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

// =================================================================================================
static void BM_StaticVectorCreation_String(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  for (auto _ : state) {
    Igor::StaticVector<std::string, CAPACITY> vec(
        512, "000000000000000000000000000000000000000000000000000000000000"s);
    benchmark::DoNotOptimize(vec.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorCopy_String(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  Igor::StaticVector<std::string, CAPACITY> v1(
      512, "000000000000000000000000000000000000000000000000000000000000"s);
  for (auto _ : state) {
    Igor::StaticVector<std::string, CAPACITY> v2(v1);
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorCopy_String_DifferentSize(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  Igor::StaticVector<std::string, 2 * CAPACITY> v1(
      512, "000000000000000000000000000000000000000000000000000000000000"s);
  for (auto _ : state) {
    Igor::StaticVector<std::string, CAPACITY> v2(v1);
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

static void BM_StaticVectorMove_String(benchmark::State& state) {
  constexpr size_t CAPACITY = 1024;
  for (auto _ : state) {
    state.PauseTiming();
    Igor::StaticVector<std::string, CAPACITY> v1(
        512, "000000000000000000000000000000000000000000000000000000000000"s);
    state.ResumeTiming();
    Igor::StaticVector<std::string, CAPACITY> v2(std::move(v1));
    benchmark::DoNotOptimize(v2.data());
    benchmark::ClobberMemory();
  }
}

// =================================================================================================
BENCHMARK(BM_StaticVectorCreation_Int);
BENCHMARK(BM_StaticVectorCopy_Int);
BENCHMARK(BM_StaticVectorCopy_Int_DifferentSize);
BENCHMARK(BM_StaticVectorMove_Int);

BENCHMARK(BM_StaticVectorCreation_String);
BENCHMARK(BM_StaticVectorCopy_String);
BENCHMARK(BM_StaticVectorCopy_String_DifferentSize);
BENCHMARK(BM_StaticVectorMove_String);

BENCHMARK_MAIN();
