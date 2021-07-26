#include "../ThreadPool.h"
#include <benchmark/benchmark.h>
#include <future>
#include <cmath>
#include <array>

constexpr int kTaskNum = 10000;
constexpr int kIteration = 50000;

static void dummyTask(int n = kIteration) {
    constexpr double kStartValue = 1.414;
    double currValue = kStartValue;
    for (int i = 0; i < n; ++i) {
        currValue = std::sin(currValue);
    }
    benchmark::DoNotOptimize<double>(currValue);
}

static void BM_DummyTask(benchmark::State& state) {
    for (auto _ : state) {
        dummyTask();
    }
}
BENCHMARK(BM_DummyTask);

static void BM_AsyncUsage(benchmark::State& state) {
    for (auto _ : state) {
        std::array<std::future<void>, kTaskNum> futures;
        for (int i = 0; i < kTaskNum; ++i) {
            futures[i] = std::async(dummyTask, kIteration);
        }
        for (auto& fu : futures) {
            fu.wait();
        }
    }
    state.SetItemsProcessed(state.iterations() * kTaskNum);
}
BENCHMARK(BM_AsyncUsage)->UseRealTime();

static void BM_ThreadPoolUsage(benchmark::State& state) {
    concurrent::ThreadPool pool;
    for (auto _ : state) {
        for (int i = 0; i < kTaskNum; ++i) {
            pool.schedule([]{ dummyTask(kIteration); });
        }
        pool.wait();
    }
    state.SetItemsProcessed(state.iterations() * kTaskNum);
}
BENCHMARK(BM_ThreadPoolUsage)->UseRealTime();

BENCHMARK_MAIN();