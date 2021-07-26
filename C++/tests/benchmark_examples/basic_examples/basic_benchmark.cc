#include <benchmark/benchmark.h>
#include <string>
#include <thread>
#include <chrono>

static void BM_StringCreation(benchmark::State& state) {
    for (auto _ : state) {
        std::string emptyString;
    }
}
BENCHMARK(BM_StringCreation);

static void BM_StringCopy(benchmark::State& state) {
    std::string str = "hello";
    for (auto _ : state) {
        std::string copy(str);
    }
}
BENCHMARK(BM_StringCopy);

static void dummyLoad() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

static void BM_DummyLoad(benchmark::State& state) {
    for (auto _ : state) {
        dummyLoad();
    }
}
BENCHMARK(BM_DummyLoad);

BENCHMARK_MAIN();